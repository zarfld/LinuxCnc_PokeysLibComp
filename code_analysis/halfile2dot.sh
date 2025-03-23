#!/bin/bash
# Generates a diagram from a HAL file for LinuxCNC
# Usage:    ./halfile2dot.sh myconfig.hal
#           dot -Tpng hal_diagram.dot -o hal_diagram.png

# Output: hal_diagram.dot (für dot -Tpng oder -Tsvg)
# Requires: sudo apt install graphviz




#!/bin/bash

HALFILE="$1"
OUTFILE="hal_diagram.dot"

if [ -z "$HALFILE" ]; then
  echo "Usage: $0 file.hal"
  exit 1
fi

echo "digraph HAL {" > "$OUTFILE"
echo "  rankdir=LR;" >> "$OUTFILE"
echo "  node [style=filled];" >> "$OUTFILE"

declare -A COMPONENTS
declare -A CLUSTERS

# 1. Komponenten erkennen (loadrt / loadusr)
grep -E '^\s*(loadrt|loadusr)' "$HALFILE" | while read -r line; do
    comp=$(echo "$line" | awk '{print $2}' | cut -d'.' -f1)
    COMPONENTS["$comp"]=1
done

# 2. Subgraph für jede Komponente
for comp in "${!COMPONENTS[@]}"; do
    echo "  subgraph cluster_$comp {" >> "$OUTFILE"
    echo "    label=\"$comp\";" >> "$OUTFILE"
    echo "    style=filled; color=lightgrey;" >> "$OUTFILE"
    CLUSTERS["$comp"]=1
    echo "  }" >> "$OUTFILE"
done

# 3. setp → Parameter-Definitionen
grep -E '^\s*setp' "$HALFILE" | while read -r line; do
    pin=$(echo "$line" | awk '{print $2}')
    comp=$(echo "$pin" | cut -d. -f1)
    echo "    \"$pin\" [shape=ellipse, fillcolor=lightpink];" >> "$OUTFILE"
    echo "    \"$comp\" -> \"$pin\" [style=dashed, color=gray];" >> "$OUTFILE"
done

# 4. net → Signalverkabelung
grep -E '^\s*net' "$HALFILE" | while read -r line; do
    sig=$(echo "$line" | awk '{print $2}')
    rest=$(echo "$line" | cut -d' ' -f3-)

    # Initiale Signaldarstellung
    echo "  \"$sig\" [shape=diamond, fillcolor=khaki];" >> "$OUTFILE"

    # Richtungs-Parsing
    for token in $rest; do
        dir=""
        pin=""

        if [[ "$token" == *'=>' ]]; then
            dir="src"
            pin="${token%=>}"
        elif [[ "$token" == *'<=' ]]; then
            dir="dst"
            pin="${token%<=}"
        elif [[ "$token" == *'<=>' ]]; then
            dir="both"
            pin="${token%<=>}"
        else
            # fallback = Quellpin, wenn kein Pfeil
            dir="src"
            pin="$token"
        fi

        # Knoten für den Pin anlegen, wenn nicht vorhanden
        shape="box"
        case "$dir" in
            src)  color="lightblue"; echo "  \"$pin\" [fillcolor=$color, shape=$shape];" >> "$OUTFILE"; echo "  \"$pin\" -> \"$sig\";" >> "$OUTFILE" ;;
            dst)  color="lightgreen"; echo "  \"$pin\" [fillcolor=$color, shape=$shape];" >> "$OUTFILE"; echo "  \"$sig\" -> \"$pin\";" >> "$OUTFILE" ;;
            both) color="khaki"; echo "  \"$pin\" [fillcolor=$color, shape=$shape];" >> "$OUTFILE"; echo "  \"$pin\" -> \"$sig\";" >> "$OUTFILE"; echo "  \"$sig\" -> \"$pin\";" >> "$OUTFILE" ;;
        esac

        # Pin der Komponente zuordnen
        comp=$(echo "$pin" | cut -d. -f1)
        if [[ -n "${CLUSTERS[$comp]}" ]]; then
            echo "  subgraph cluster_$comp { \"$pin\"; }" >> "$OUTFILE"
        fi
    done
done

echo "}" >> "$OUTFILE"

echo "DOT-Datei erstellt: $OUTFILE"
echo "Zum Rendern:"
echo "  dot -Tpng $OUTFILE -o hal_diagram.png"

