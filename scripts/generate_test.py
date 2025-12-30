"""
generate_test.py - Generate random test cases for exchange simulator
"""

import random
import argparse
from pathlib import Path

# generate a random test file with commands
def generate_test_file(num_orders=1000, output_file="test_input.txt", 
                       price_range=(100, 200), qty_range=(1, 100),
                       seed=None):

    if seed is not None:
        random.seed(seed)
    
    existing_order_ids = set()
    active_order_ids = []  # Track orders that can be cancelled
    commands = []
    next_order_id = 1
    
    for i in range(num_orders):
        # 70% new orders, 20% cancels, 5% top of book, 5% full book
        rand = random.random()
        
        if rand < 0.70:  # New order
            order_id = next_order_id
            next_order_id += 1
            side = random.choice(['B', 'S'])
            price = random.randint(price_range[0], price_range[1])
            qty = random.randint(qty_range[0], qty_range[1])
            
            commands.append(f"N {order_id} {side} {price} {qty}")
            active_order_ids.append(order_id)
            
        elif rand < 0.90 and active_order_ids:  # Cancel order
            order_id = random.choice(active_order_ids)
            commands.append(f"C {order_id}")
            active_order_ids.remove(order_id)
            
        elif rand < 0.95:  # Print top of book
            commands.append("P")
            
        else:  # Print full book
            commands.append("B")
    
    # Write to file
    with open(output_file, 'w') as f:
        for cmd in commands:
            f.write(cmd + '\n')
        f.write("X\n")
    
    print(f"Generated {len(commands)} commands in {output_file}")
    print(f"  - Orders: {len([c for c in commands if c.startswith('N')])}")
    print(f"  - Cancels: {len([c for c in commands if c.startswith('C')])}")
    print(f"  - Queries: {len([c for c in commands if c in ['P', 'B']])}")

def main():
    parser = argparse.ArgumentParser(description='Generate random test cases')
    parser.add_argument('-n', '--num-orders', type=int, default=100,
                       help='Number of orders to generate (default: 100)')
    parser.add_argument('-o', '--output', type=str, default='test_input.txt',
                       help='Output file path (default: test_input.txt)')
    parser.add_argument('--price-min', type=int, default=50,
                       help='Minimum price (default: 50)')
    parser.add_argument('--price-max', type=int, default=200,
                       help='Maximum price (default: 200)')
    parser.add_argument('--qty-min', type=int, default=1,
                       help='Minimum quantity (default: 1)')
    parser.add_argument('--qty-max', type=int, default=100,
                       help='Maximum quantity (default: 100)')
    parser.add_argument('--seed', type=int, default=None,
                       help='Random seed for reproducibility')
    
    args = parser.parse_args()
    
    generate_test_file(
        num_orders=args.num_orders,
        output_file=args.output,
        price_range=(args.price_min, args.price_max),
        qty_range=(args.qty_min, args.qty_max),
        seed=args.seed
    )

if __name__ == '__main__':
    main()