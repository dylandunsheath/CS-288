import requests
import time
from bs4 import BeautifulSoup
from pymongo import MongoClient

start_time = time.time()

url = 'https://finance.yahoo.com/most-active'
# MongoDB Connection
client = MongoClient('localhost', 27017)
# Creates database
db = client['stock_database']
# Creates collection within database
collection = db['most_active_stocks']

def getStocks(url):
    response = requests.get(url)
    # Successful request
    if response.status_code == 200:
        html = response.text
        soup = BeautifulSoup(html, 'html.parser')
        # Find table
        table = soup.find('table', {'class': 'W(100%)'})
        # Set to add the 'Symbols' found on site
        symbols_on_website = set()
        # Ignore headers when looping
        for row in table.find_all('tr')[1:]:
            columns = row.find_all('td')
            # Gets the data we need
            symbol = columns[0].text
            name = columns[1].text
            price = columns[2].text
            change = columns[3].text
            volume = columns[5].text
            # Adds symbols on website to set
            # (all unique)
            symbols_on_website.add(symbol)
            # Dictionary (Key->Value)            
            doc = {
                'Symbol': symbol,
                'Name': name,
                'Price': price,
                'Change': change,
                'Volume': volume,
            }
            
            # Document doesn't exist in database
            if collection.find_one({'Symbol': symbol}) is None:
                collection.insert_one(doc)
                print("Symbol: " + symbol + " added")
            # Document exists in database
            else:
                # Define the update operation and criteria
                update_operation = {'$set': {'Price': price, 'Change': change, 'Volume': volume}}
                update_criteria = {'Symbol': symbol}
                # Perform the update operation
                collection.update_one(update_criteria, update_operation)
                print("Symbol: " + symbol + " updated")
        # Checks if any companies/Symbols are no longer on the website
        for doc in collection.find():
            symbol_in_db = doc['Symbol']
            # If the symbol we added previously is no longer on wesite, delete it from collection
            if symbol_in_db not in symbols_on_website:
                collection.delete_one({'Symbol': symbol_in_db})
                print("Symbol: " + symbol_in_db + " deleted")
    # Retry Request
    else:
        print("Failed to retrieve data. Retrying...")
        response = requests.get(url)        
        if response.status_code == 200:
            html = response.text
            soup = BeautifulSoup(html, 'html.parser')
            # Find table
            table = soup.find('table', {'class': 'W(100%)'})
            symbols_on_website = set()
            # Ignore headers when looping
            for row in table.find_all('tr')[1:]:
                columns = row.find_all('td')
                # Gets the data we need
                symbol = columns[0].text
                name = columns[1].text
                price = columns[2].text
                change = columns[3].text
                volume = columns[5].text
                
                symbols_on_website.add(symbol)
                
                doc = {
                    'Symbol': symbol,
                    'Name': name,
                    'Price': price,
                    'Change': change,
                    'Volume': volume,
                }
                # Document doesn't exist in database
                if collection.find_one({'Symbol': symbol}) is None:
                    collection.insert_one(doc)
                    print("Symbol: " + symbol + " added")
                # Document exists
                else:
                    # Define the update operation and criteria
                    update_operation = {'$set': {'Price': price, 'Change': change, 'Volume': volume}}
                    update_criteria = {'Symbol': symbol}
                    # Perform the update operation
                    collection.update_one(update_criteria, update_operation)
                    print("Symbol: " + symbol + " updated")
            for doc in collection.find():
                symbol_in_db = doc['Symbol']
                if symbol_in_db not in symbols_on_website:
                    collection.delete_one({'Symbol': symbol_in_db})
                    print("Symbol: " + symbol_in_db + " deleted")
        else:
            # Retry fails
            print("Retry failed... Exiting program....")
            exit()    

while True:
    elapsed_time = time.time() - start_time
    if elapsed_time >= 900:
        print("15 minutes have passed...")
        exit()
    getStocks(url)
    # Kills the program after 15 minutes
    print('Waiting 3 minutes...')
    time.sleep(180)