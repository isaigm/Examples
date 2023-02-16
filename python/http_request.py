import requests
import time
API_KEY = "your_api_key"
FROM_CURRENCY = "DOGE"
TO_CURRENCY = "USD"
params = dict(function="CURRENCY_EXCHANGE_RATE", from_currency=FROM_CURRENCY, 
                to_currency=TO_CURRENCY, apikey=API_KEY)
last_exch_rate = 0
while True:
    response = requests.get("https://www.alphavantage.co/query?", params=params)
    json = response.json()
    curr_exch_rate = float(json['Realtime Currency Exchange Rate']['5. Exchange Rate'])
 
    if curr_exch_rate > last_exch_rate:
        print(f"stonks {curr_exch_rate:5f}")
        print("\a", end='')
    elif curr_exch_rate < last_exch_rate:
        print(f"not stonks {curr_exch_rate:5f}")
    last_exch_rate = curr_exch_rate
    time.sleep(30)
