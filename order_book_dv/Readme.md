# Submitted By 
```
 Name - Sekhar Barua
 Email - sekhar.barua@gmail.com
```
# Info
This Feed handler purpose is parse the order messages.

# API Implented

1. OrderCounts()
    o Counts the total number of orders per symbol.

2. BiggestBuyOrders(symbol=”DVAM1”)
    o Finds the top 3 biggest BUY orders in terms of volume for a specific symbol.

3. BestSellAtTime(symbol=”DVAM1”, timestamp=”15:30:00”)
   o Finds the best SELL price and related order volume for a specific symbol at a given time.


# How to Run
```
Note: Code written in Linux L-11554 20.04.3 LTS (Focal Fossa) (Ubuntu 20.4)
1. copy this project(folder order_book) and go to the folder location in terminal
2. cmake . .
3. make
4. ./run.sh (This script take the `data/mesage.dat` which contains order messages)

```
# How to feed messages
In path `data/mesage.dat` add the messages to get feed into Feed Handler.
