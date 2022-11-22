select distinct Shipper.CompanyName,DelayRate 
from 'Order',
(	
	select Tar.ShipVia,Tar.Lcnt,Total.Tcnt,
	        round((Tar.Lcnt * 1.0/Total.Tcnt),2) as DelayRate 
	from(
		select ShipVia,count(Id) as Lcnt
		from 'Order' 
		where ShippedDate > RequiredDate 
		group by ShipVia) as Tar, 
	     (
	     	select ShipVia,count(Id) as Tcnt 
		from 'Order' 
		group by ShipVia) as Total
	where Tar.ShipVia = Total.ShipVia 
)as Res,Shipper
where Shipper.Id = 'Order'.ShipVia and Res.ShipVia = 'Order'.ShipVia
order by DelayRate desc;
