#!/usr/bin/env python3.13
import sys

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <input_file> <name_count>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    try:
        name_count = int(sys.argv[2])
        if name_count < 0:
            raise ValueError
    except ValueError:
        print("Second parameter must be a non-negative integer")
        sys.exit(1)
    matching_students = []
    
    try:
        with open(input_file, 'r') as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                parts = line.split(', ', 1)
                if len(parts) != 2:
                    continue
                
                student_number, full_name = parts
                name_parts = full_name.split()
                if not name_parts:
                    continue
                
                last_name = name_parts[-1]
                first_middle_names = name_parts[:-1]
                
                if len(first_middle_names) == name_count:
                    matching_students.append({
                        'student_number': student_number,
                        'last_name': last_name
                    })
    
    except FileNotFoundError:
        print(f"Cannot open file {input_file}")
        sys.exit(1)
    
    if not matching_students:
        print("None found")
    elif len(matching_students) == 1:
        print(matching_students[0]['student_number'])
    else:
        matching_students.sort(key=lambda x: x['last_name'])
        print(matching_students[0]['student_number'])

if __name__ == "__main__":
    main()