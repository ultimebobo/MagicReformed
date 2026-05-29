#!/usr/bin/env python3
"""
Convert MagicReformed spell logs to CSV format

Usage:
    python spell_logs_to_csv.py <log_file> [output_csv]

Example:
    python spell_logs_to_csv.py MagicReformed.log spells.csv
    python spell_logs_to_csv.py MagicReformed.log  # Outputs: MagicReformed_spells.csv
"""

import sys
import csv
import re
from pathlib import Path


def parse_spell_logs(log_file):
    """
    Parse spell log entries from a log file.
    
    Expected format:
    SPELL_LOG||Name|School|Level|Delivery|Element|Cost|Other
    """
    spells = []
    
    try:
        with open(log_file, 'r', encoding='utf-8', errors='replace') as f:
            for line in f:
                # Look for lines containing SPELL_LOG entries
                if 'SPELL_LOG|' not in line:
                    continue
                
                # Extract the SPELL_LOG part (skip timestamp and logger stuff)
                match = re.search(r'SPELL_LOG\|(.+?)(?:\s*$|\n)', line)
                if not match:
                    continue
                
                content = match.group(1)
                
                # Skip header line
                if content.startswith('Name|School|'):
                    continue
                
                # Parse the pipe-separated values
                parts = content.split('|')
                
                # Expecting: Name|School|Level|Delivery|Element|Cost|Other
                if len(parts) >= 7:
                    spell_data = {
                        'Name': parts[0].strip(),
                        'School': parts[1].strip(),
                        'Level': parts[2].strip(),
                        'Delivery': parts[3].strip(),
                        'Element': parts[4].strip(),
                        'Cost': parts[5].strip(),
                        'Other': parts[6].strip()
                    }
                    spells.append(spell_data)
    
    except FileNotFoundError:
        print(f"Error: Log file '{log_file}' not found.", file=sys.stderr)
        return None
    except Exception as e:
        print(f"Error reading log file: {e}", file=sys.stderr)
        return None
    
    return spells


def write_csv(spells, output_file):
    """Write parsed spell data to CSV file."""
    if not spells:
        print("No spell data found to write.", file=sys.stderr)
        return False
    
    try:
        fieldnames = ['Name', 'School', 'Level', 'Delivery', 'Element', 'Cost', 'Other']
        
        with open(output_file, 'w', newline='', encoding='utf-8') as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(spells)
        
        print(f"Successfully wrote {len(spells)} spells to '{output_file}'")
        return True
    
    except Exception as e:
        print(f"Error writing CSV file: {e}", file=sys.stderr)
        return False


def main():
    if len(sys.argv) < 2:
        print("Usage: python spell_logs_to_csv.py <log_file> [output_csv]")
        print("Example: python spell_logs_to_csv.py MagicReformed.log spells.csv")
        sys.exit(1)
    
    log_file = sys.argv[1]
    
    # Determine output file name
    if len(sys.argv) >= 3:
        output_file = sys.argv[2]
    else:
        log_path = Path(log_file)
        output_file = log_path.parent / f"{log_path.stem}_spells.csv"
    
    print(f"Reading spell logs from: {log_file}")
    
    # Parse logs
    spells = parse_spell_logs(log_file)
    if spells is None:
        sys.exit(1)
    
    if not spells:
        print("No spell entries found in log file.")
        sys.exit(1)
    
    print(f"Found {len(spells)} spell entries")
    
    # Write CSV
    if write_csv(spells, output_file):
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == '__main__':
    main()
