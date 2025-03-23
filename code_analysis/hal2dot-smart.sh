#!/bin/bash

# Usage: ./hal2dot-smart.sh mycomponent.c [another.c ...]
# Default: alle .c-Dateien im Verzeichnis
#
# Requires: dot (Graphviz): sudo apt install graphviz oder brew install graphviz

SRC_FILES=${@:-*.c}
OUTPUT_FILE="hal_component.dot"

# Farben je Typ
COLOR_IN="lightblue"
COLOR_OUT="lightgreen"
COLOR_IO="khaki"
COLOR_PARAM="lightpink"

echo "digraph HALComponent {" > "$OUTPUT_FILE"
echo "  rankdir=LR;" >> "$OUTPUT_FILE"
echo "  node [shape=box, style=filled];" >> "$OUTPUT_FILE"

# HashMap (simuliert) für Komponenten
declare -A COMPONENTS

# Pins extrahieren
grep -hoP 'hal_pin_new\s*\(\s*"[^"]+"\s*,\s*HAL_[A-Z_]+' $SRC_FILES | while read -r line; do
    name=$(echo "$line" | grep -oP '"[^"]+"' | tr -d '"')
    type=$(echo "$line" | grep -oP 'HAL_[A-Z_]+')

    # Komponentennamen extrahieren (z. B. "mycomp.in" → "mycomp")
    comp=$(echo "$name" | cut -d. -f1)

    COMPONENTS["$comp"]=1

    case "$type" in
        HAL_IN)  color=$COLOR_IN ;;
        HAL_OUT) color=$COLOR_OUT ;;
        HAL_IO)  color=$COLOR_IO ;;
        *)       color=gray ;;
    esac

    echo "  subgraph cluster_$comp {" >> "$OUTPUT_FILE"
    echo "    label=\"$comp\";" >> "$OUTPUT_FILE"
    echo "    \"$name\" [fillcolor=$color];" >> "$OUTPUT_FILE"
    echo "  }" >> "$OUTPUT_FILE"
done

# Parameter extrahieren
grep -hoP 'hal_param_new\s*\(\s*"[^"]+"\s*,\s*HAL_[A-Z_]+' $SRC_FILES | while read -r line; do
    name=$(echo "$line" | grep -oP '"[^"]+"' | tr -d '"')
    type=$(echo "$line" | grep -oP 'HAL_[A-Z_]+')

    comp=$(echo "$name" | cut -d. -f1)

    COMPONENTS["$comp"]=1

    echo "  subgraph cluster_$comp {" >> "$OUTPUT_FILE"
    echo "    label=\"$comp\";" >> "$OUTPUT_FILE"
    echo "    \"$name\" [fillcolor=$COLOR_PARAM, shape=ellipse];" >> "$OUTPUT_FILE"
    echo "  }" >> "$OUTPUT_FILE"
done

echo "}" >> "$OUTPUT_FILE"

echo "Erstellt: $OUTPUT_FILE"
echo "Du kannst es rendern mit:"
echo "  dot -Tpng $OUTPUT_FILE -o hal_component.png"
