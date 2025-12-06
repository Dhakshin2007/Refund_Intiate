import sys

class DeliveryPartner:
    def __init__(self, id, dist_to_consumer):
        self.id = id
        self.dist_to_consumer = dist_to_consumer

class Warehouse:
    def __init__(self, id, dist_from_consumer, traffic_percent):
        self.id = id
        self.dist_from_consumer = dist_from_consumer
        self.traffic_percent = traffic_percent

def get_valid_number(prompt, data_type=float):
    while True:
        try:
            value = data_type(input(prompt))
            if value < 0:
                print("Value cannot be negative. Try again.")
            else:
                return value
        except ValueError:
            print("Invalid input. Please enter a number.")

def find_optimal_return_path(partners, warehouses):
    # --- Step 1: Find the Nearest Delivery Partner ---
    sorted_partners = sorted(partners, key=lambda x: x.dist_to_consumer)
    
    if not sorted_partners:
        return None
    
    best_partner = sorted_partners[0]
    
    print(f"\n[ANALYSIS] Nearest Delivery Partner Identified: ID {best_partner.id}")
    print(f"           Pickup Distance: {best_partner.dist_to_consumer} km")

    # --- Step 2: Find the Best Warehouse (Considering Traffic) ---
    best_warehouse = None
    min_effective_cost = float('inf')
    best_total_distance = 0

    print(f"\n[ANALYSIS] Calculating Effective Costs (Distance + Traffic Penalty)...")
    
    for warehouse in warehouses:
        # Cost Logic: Distance * (1 + Traffic%)
        # Example: 10km with 40% traffic = 10 * 1.4 = 14 cost units
        traffic_multiplier = 1 + (warehouse.traffic_percent / 100.0)
        return_leg_cost = warehouse.dist_from_consumer * traffic_multiplier
        
        # Total Cost = Pickup Distance + Return Leg Cost
        total_effective_cost = best_partner.dist_to_consumer + return_leg_cost
        
        print(f"  > Warehouse {warehouse.id}:")
        print(f"    - Physical Dist: {warehouse.dist_from_consumer} km")
        print(f"    - Traffic Load:  {warehouse.traffic_percent}%")
        print(f"    - Calculated Score: {total_effective_cost:.2f}")

        if total_effective_cost < min_effective_cost:
            min_effective_cost = total_effective_cost
            best_warehouse = warehouse
            best_total_distance = best_partner.dist_to_consumer + warehouse.dist_from_consumer

    return {
        "partner": best_partner.id,
        "warehouse": best_warehouse.id,
        "total_distance": best_total_distance,
        "score": min_effective_cost
    }

def main():
    print("=== LOGISTICS REFUND OPTIMIZER (Run Time Mode) ===")
    
    # 1. Input Delivery Partners
    partners = []
    num_partners = get_valid_number("Enter number of Delivery Partners available: ", int)
    
    print("--- Enter Details for Partners ---")
    for i in range(num_partners):
        dist = get_valid_number(f"Distance for Partner {i+1} to Consumer (km): ")
        partners.append(DeliveryPartner(id=i+1, dist_to_consumer=dist))

    # 2. Input Warehouses
    warehouses = []
    num_warehouses = get_valid_number("\nEnter number of Warehouses nearby: ", int)
    
    print("--- Enter Details for Warehouses ---")
    for i in range(num_warehouses):
        # We use Letters (A, B, C...) for Warehouse IDs
        w_id = chr(65 + i) 
        dist = get_valid_number(f"Distance from Consumer to Warehouse {w_id} (km): ")
        traffic = get_valid_number(f"Traffic Percentage for path to Warehouse {w_id} (0-100): ")
        warehouses.append(Warehouse(id=w_id, dist_from_consumer=dist, traffic_percent=traffic))

    # 3. Process
    result = find_optimal_return_path(partners, warehouses)

    if result:
        print("\n=========================================")
        print("OPTIMAL DECISION:")
        print(f"1. Assign Delivery Partner: {result['partner']}")
        print(f"2. Route to Warehouse:      {result['warehouse']}")
        print(f"3. Total Travel Distance:   {result['total_distance']} km")
        print(f"4. Optimization Score:      {result['score']:.2f} (Lower is better)")
        print("=========================================")
    else:
        print("Error: No data provided.")

if __name__ == "__main__":
    main()