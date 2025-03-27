import os
import re

def extract_comments(file_path):
    """Extracts all /* ... */ comment blocks from a file."""
    with open(file_path, 'r', encoding='utf-8') as file:
        content = file.read()
    comments = re.findall(r'/\*.*?\*/', content, re.DOTALL)
    return comments

def find_dot_file(source_file, dot_files):
    """Finds a corresponding .dot file for a given source file."""
    base_name = os.path.splitext(source_file)[0]
    for dot_file in dot_files:
        if base_name in dot_file:
            return dot_file
    return None

def generate_dox_files(source_dir):
    """Generates .dox files for all .c and .h files in the specified directory."""
    dot_files = [f for f in os.listdir(source_dir) if f.endswith('.dot')]
    
    for root, _, files in os.walk(source_dir):
        for file in files:
            if file.endswith(('.c', '.h')):
                file_path = os.path.join(root, file)
                comments = extract_comments(file_path)
                if comments:
                    dox_file_name = f"{os.path.splitext(file)[0]}.dox"
                    dox_file_path = os.path.join(root, dox_file_name)
                    
                    with open(dox_file_path, 'w', encoding='utf-8') as dox_file:
                        dox_file.write(f"/// \\file {file}\n")
                        dox_file.write(f"/// \\brief Documentation for {file}\n\n")
                        dox_file.write("/// \\details\n")
                        for comment in comments:
                            dox_file.write(comment + "\n\n")
                        dox_file.write(f"/// \\see {file}\n")
                        
                        # Check for corresponding .dot file
                        dot_file = find_dot_file(file, dot_files)
                        if dot_file:
                            dox_file.write(f"/// \\dot\n/// \\include {dot_file}\n/// \\enddot\n")
                    
                    print(f"Generated {dox_file_path}")

# Specify the path to your source directory
source_directory = '../external/Linuxcnc/emc/motion/'
generate_dox_files(source_directory)

source_directory = '../external/Linuxcnc/hal/'
generate_dox_files(source_directory)

source_directory = '../external/Linuxcnc/rtapi/'
generate_dox_files(source_directory)