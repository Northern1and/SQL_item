with pdt as(
    select ProductName,Product.Id
    from 'Order'
    join Customer on CustomerId = Customer.Id
    join OrderDetail on 'Order'.Id = OrderDetail.OrderId
    join Product on OrderDetail.ProductId = Product.Id
    where CompanyName = 'Queen Cozinha' AND Date(OrderDate) = '2014-12-25'
),
cnt as (
    select row_number() over (order by pdt.id asc) as num,
    pdt.ProductName as name
    from pdt
),
flattened as(
    select num,name as name 
    from cnt 
    where num = 1
    
    union all 
    
    select cnt.num,f.name || ',' || cnt.name 
    from cnt 
    join flattened f on cnt.num = f.num + 1
)

select name 
from flattened
ORDER BY num DESC 
limit 1;
