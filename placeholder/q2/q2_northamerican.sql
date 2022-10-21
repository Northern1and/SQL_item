select Id, ShipCountry, 
       case 
              when ShipCountry in ('USA', 'Mexico','Canada')
              then 'NorthAmerica'
              else 'OtherPlace'
       END
from 'Order'
where Id >= 15445
ORDER BY Id ASC
LIMIT 20;
