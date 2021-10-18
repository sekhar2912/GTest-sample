# Find the total ordered quantity of a product for every weekday

1. Implement missing methods in the `Analyzer` class so that its `solve` method
returns the `map` with the weekday (in `std::string` form) as the key.

2. Implement missing methods in the `Analyzer` class so that its `solve` method
returns the sum of ordered quantity for a given product id as a value. 

3. The method should always return a `map`. If there are no orders for the
passed `productId` for some weekday, there should exist an entry for this day
with a `0` value.

## Example output

Your implementation should return following output for **productId=9872** for example data:

```json
{
  "MONDAY" : 3, 
  "TUESDAY" : 0, 
  "WEDNESDAY" : 0, 
  "THURSDAY" : 0, 
  "FRIDAY"  : 0, 
  "SATURDAY" : 5,
  "SUNDAY" : 4 
}
```

In the example above, there are two orders placed on Saturday:  with `3` and `2` quantities, so the sum = `5`. For Monday there is only one 
order, so total quantity equals to `3`.

## Example input

Orders collection:
```javascript
[
    {
        orderId: 554,
        creationDate: "2017-03-25T10:35:20", // Saturday
        orderLines: [
            {productId: 9872, name: 'Pencil', quantity: 3, unitPrice: 3.00}
        ]
    },
    {
        orderId: 555,
        creationDate: "2017-03-25T11:24:20", // Saturday
        orderLines: [
            {productId: 9872, name: 'Pencil', quantity: 2, unitPrice: 3.00},
            {productId: 1746, name: 'Eraser', quantity: 1, unitPrice: 1.00}
        ]
    },
    {
        orderId: 453,
        creationDate: "2017-03-27T14:53:12", // Monday
        orderLines: [
            {productId: 5723, name: 'Pen', quantity: 4, unitPrice: 4.22},
            {productId: 9872, name: 'Pencil', quantity: 3, unitPrice: 3.12},
            {productId: 3433, name: 'Erasers Set', quantity: 1, unitPrice: 6.15}
        ]
    },
    {
        orderId: 431,
        creationDate: "2017-03-20T12:15:02", // Monday
        orderLines: [
            {productId: 5723, name: 'Pen', quantity: 7, unitPrice: 4.22},
            {productId: 3433, name: 'Erasers Set', quantity: 2, unitPrice: 6.15}
        ]
    },
    {
        orderId: 690,
        creationDate: "2017-03-26T11:14:00", // Sunday
        orderLines: [
            {productId: 9872, name: 'Pencil', quantity: 4, unitPrice: 3.12},
            {productId: 4098, name: 'Marker', quantity: 5, unitPrice: 4.50}
        ]
    }
];
```
