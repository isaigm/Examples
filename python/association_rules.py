import csv

def support_count(transactions: list[set], item_set: set):
    count = 0
    for transaction in transactions:
        if transaction.intersection(item_set) == item_set:
            count += 1
    return count

def support(transactions: list[set], item_set: set):
    return support_count(transactions, item_set) / len(transactions)

def confidence(transactions: list[set], x: set, y: set):
    return support_count(transactions, x | y) / support_count(transactions, x)

def all_pairs(items: list):
    for i in range(len(items) - 1):
        for j in range(i + 1, len(items)):
            yield {items[i], items[j]}

def gen_all_assocciation_rules(items_set: list):
    for n in range(1, 2 ** len(items_set) - 1):
        lhs = set()
        j = 0
        while n > 0:
            if n & 1 == 1:
                lhs.add(items_set[j])
            j += 1
            n >>= 1
        yield (lhs, set(items_set) - lhs)

def apriori(items: set, transactions: list[set], min_sup, min_conf):
    curr_item_sets = []
    item_set_seen = set()
    def get_hash(item_set: list):
        item_set.sort()
        return str(item_set)
    
    for pair in all_pairs(list(items)):
        if support_count(transactions, pair) >= min_sup:
            curr_item_sets.append(pair)
            item_set_seen.add(get_hash(list(pair)))
    
    all_items_sets = []
    all_items_sets.extend(curr_item_sets)
    for _ in range(3, len(items)):
        items_sets = []
        for item_set in curr_item_sets:
            remaining_items = items - item_set
            for item in remaining_items:
                new_item_set = item_set | {item}
                custom_hash = get_hash(list(new_item_set))
                if custom_hash not in item_set_seen and support_count(transactions, new_item_set) >= min_sup:
                    items_sets.append(new_item_set)
                    item_set_seen.add(custom_hash)
        
        if len(items_sets) == 0:
            break
        curr_item_sets = items_sets
        all_items_sets.extend(items_sets)
  

with open('C:\\Users\\isaig\\Downloads\\GroceryStoreDataSet.csv', newline='\n') as csvfile:
    data_reader  = csv.reader(csvfile, delimiter=',')
    items        = set()
    transactions = []
    for row in data_reader:
        transaction = set(row[0].split(','))
        transactions.append(transaction)
        items |= transaction
        
    apriori(items, transactions, 1, 0.5)
   
   
