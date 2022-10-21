select 	CategoryName,
	count(*) as CategoryCount,
	round(avg(UnitPrice),2) as AvgUnitPrice,
	min(UnitPrice) as MinUnitPrice,
	max(UnitPrice) as MaxUnitPrice,
	sum(UnitsOnOrder) as TotalUnitsOnOrder
from Product INNER JOIN Category on Product.CategoryId = Category.Id
group by CategoryId
HAVING CategoryCount > 10
Order BY CategoryId;
