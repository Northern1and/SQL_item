select RegionDescription, FirstName, LastName, birth
from 
(
    select RegionId  as rid,EmployeeId,FirstName,LastName,Birthdate,
                        MAX(Employee.Birthdate) as birth
    from Employee
    join EmployeeTerritory on Employee.Id = EmployeeTerritory.EmployeeId
    join Territory on TerritoryId = Territory.Id
    GROUP BY RegionId
)
join Region on Region.Id = rid
GROUP BY EmployeeId
ORDER BY rid;
