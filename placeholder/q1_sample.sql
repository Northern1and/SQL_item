select ShipName as "Bottom-Dollar Markets",substr(ShipName,1,instr(ShipName,'-')-1) as Bottom from "Order" 
where ShipName like '%-%';
