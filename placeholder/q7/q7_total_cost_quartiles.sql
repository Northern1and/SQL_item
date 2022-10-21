WITH Target AS( 
    select 
        IFNULL(c.CompanyName,'MISSING_NAME') as CompanyName,
        o.CustomerId,
        ROUND(SUM(od.Quantity*od.UnitPrice),2) as TotalCost
    from 'Order' AS o
    join OrderDetail  od on od.OrderId = o.Id
    LEFT join  Customer c on c.Id = o.CustomerId
    GROUP BY o.CustomerId
),
quartiles AS ( 
    select * ,NTILE(4) OVER (ORDER BY TotalCost ASC) as ExpenditureQuartile
    from Target
)
SELECT CompanyName,CustomerId,TotalCost
FROM quartiles
WHERE ExpenditureQuartile = 1
ORDER BY TotalCost ASC;
