select ProductName,CompanyName,ContactName 
from(
	select ProductName,min(OrderDate),CompanyName,ContactName 
	from(
		select Id,ProductName 
		from  Product 
		where Discontinued = 1
	)as Pdt
	join OrderDetail as Dtl on Dtl.ProductId = Pdt.Id 
	join 'Order' as O on O.Id = Dtl.OrderId 		
	join Customer on Customer.Id = O.CustomerId
	group by ProductName
	)
Order by ProductName asc;
