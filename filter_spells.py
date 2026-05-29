#!/usr/bin/env python3
"""
Filter spells from MagicReformed CSV by various criteria.

Usage:
    python filter_spells.py <csv_file> [--school SCHOOL] [--level LEVEL] [--delivery DELIVERY] [--element ELEMENT]
    python filter_spells.py spells.csv -s Restoration -l Novice
    python filter_spells.py spells.csv --school Conjuration

Outputs: Names of all spells matching the filter criteria (one per line)
"""

import sys
import csv
import argparse
from pathlib import Path


# Define filters as a list of tuples: (long_name, short_name, csv_column_name)
# This makes it easy to add new filters
FILTERS = [
    ('school', 's', 'School'),
    ('level', 'l', 'Level'),
    ('delivery', 'd', 'Delivery'),
    ('element', 'e', 'Element'),
]


def create_argument_parser():
    """Create and configure the argument parser."""
    parser = argparse.ArgumentParser(
        description='Filter spells from MagicReformed CSV by various criteria.',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''Examples:
  python filter_spells.py spells.csv -s Restoration
  python filter_spells.py spells.csv --school Conjuration --level Novice
  python filter_spells.py spells.csv -s Alteration -d Self'''
    )
    
    parser.add_argument('csv_file', help='Path to the spell CSV file')
    
    # Dynamically add filter arguments based on FILTERS list
    for long_name, short_name, csv_column in FILTERS:
        parser.add_argument(
            f'--{long_name}',
            f'-{short_name}',
            dest=long_name,
            help=f'Filter by {csv_column.lower()} ({long_name.upper()})'
        )
    
    return parser


def read_spells_csv(csv_file):
    """Read and return spell data from CSV file."""
    try:
        with open(csv_file, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            spells = list(reader)
        return spells
    except FileNotFoundError:
        print(f"Error: CSV file '{csv_file}' not found.", file=sys.stderr)
        return None
    except Exception as e:
        print(f"Error reading CSV file: {e}", file=sys.stderr)
        return None


def filter_spells(spells, filters):
    """
    Filter spells based on the provided criteria.
    
    Args:
        spells: List of spell dictionaries
        filters: Dictionary of filter_name -> filter_value
    
    Returns:
        Filtered list of spells
    """
    filtered = spells
    
    # Apply each active filter
    for long_name, _, csv_column in FILTERS:
        filter_value = filters.get(long_name)
        if filter_value:
            filtered = [
                spell for spell in filtered
                if spell.get(csv_column, '').lower() == filter_value.lower()
            ]
    
    return filtered


def main():
    parser = create_argument_parser()
    args = parser.parse_args()
    
    # Check if CSV file exists
    if not Path(args.csv_file).exists():
        print(f"Error: CSV file '{args.csv_file}' not found.", file=sys.stderr)
        sys.exit(1)
    
    # Read spells from CSV
    spells = read_spells_csv(args.csv_file)
    if spells is None:
        sys.exit(1)
    
    if not spells:
        print("No spells found in CSV file.", file=sys.stderr)
        sys.exit(1)
    
    # Build filter dictionary from parsed arguments
    filters = {}
    for long_name, _, _ in FILTERS:
        value = getattr(args, long_name, None)
        if value:
            filters[long_name] = value
    
    # Filter spells
    results = filter_spells(spells, filters)
    
    # Output matching spell names
    if results:
        for spell in results:
            print(spell.get('Name', 'UNKNOWN'))
    else:
        if filters:
            print(f"No spells found matching the criteria.", file=sys.stderr)
        else:
            print("No filter criteria provided. Use -h for help.", file=sys.stderr)
        sys.exit(1)


if __name__ == '__main__':
    main()
