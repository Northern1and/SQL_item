select	Id, 
	OrderDate,
	PreDate,
       	round(julianday(OrderDate) - julianday(PreDate),2) 
from (
	select 	Id,
		OrderDate,
       		LAG(OrderDate,1,OrderDate) 
       		OVER (ORDER BY OrderDate ASC) as PreDate
	from 'Order'
	where CustomerId = 'BLONP'
	ORDER BY OrderDate ASC
	LIMIT 10
)
