--1
SELECT COUNT(*)
FROM employees
WHERE LOWER(first_name) LIKE 'k%';

--2
SELECT employee_id, first_name, last_name
FROM (SELECT employee_id, first_name, last_name
      FROM employees
      ORDER BY salary)
WHERE ROWNUM = 1;

--3
SELECT DISTINCT e.employee_id, e.last_name, e.first_name
FROM employees e
WHERE e.employee_id IN (SELECT manager_id
                        FROM employees
                        WHERE department_id = 30)
ORDER BY last_name, first_name;

--4
SELECT employee_id, last_name, first_name, (SELECT COUNT(*)
                                            FROM employees
                                            WHERE manager_id = e.employee_id) as "numar subalterni"
FROM employees e;

--5
SELECT employee_id, last_name, first_name
FROM employees e
WHERE (SELECT COUNT(*)
       FROM employees
       WHERE employee_id != e.employee_id AND e.last_name = last_name) > 0;
       
--6
SELECT department_id, department_name
FROM departments d
WHERE (SELECT COUNT(DISTINCT job_id)
       FROM employees
       WHERE department_id = d.department_id) > 1;
       
--7
SELECT prod_id, prod_desc, (SELECT SUM(qty)
                            FROM orders_tbl
                            WHERE prod_id = p.prod_id
                            GROUP BY prod_id) as "CANTITATE"
FROM products_tbl p
WHERE LOWER(prod_desc) LIKE '%plastic%';

--8
(SELECT last_name || ' ' || first_name "Nume", 'Angajat' "Tip"
FROM employee_tbl
UNION 
SELECT cust_name "Nume", 'Client' "Tip"
FROM customer_tbl)
ORDER BY "Tip", "Nume";

--9
SELECT p.prod_desc
FROM products_tbl p JOIN orders_tbl s USING (prod_id)
WHERE s.sales_rep IN (SELECT s.sales_rep
                      FROM orders_tbl s JOIN products_tbl p USING (prod_id)
                      WHERE prod_desc LIKE '% P%');

--10
select EXTRACT(day from (SELECT ord_date from orders_tbl where ord_num = '16C17')) "hey"
from dual;


SELECT DISTINCT cust_name
FROM customer_tbl c
WHERE (SELECT COUNT(*)
       FROM orders_tbl
       WHERE cust_id = c.cust_id AND EXTRACT(day from ord_date) = 17) > 0;
       
--11
SELECT last_name, first_name, bonus
FROM employee_pay_tbl ep, employee_tbl et
WHERE et.emp_id = ep.emp_id AND salary < 32000 AND bonus * 17 < 32000;
 
-- 12
SELECT last_name, first_name, NVL(SUM(o.qty), 0) as "Cantitate"
FROM employee_tbl e LEFT JOIN orders_tbl o ON o.sales_rep = e.emp_id
GROUP BY e.emp_id, e.last_name, e.first_name
HAVING SUM(o.qty) > 50 OR NVL(SUM(o.qty), 0) = 0;

-- 13
SELECT last_name, first_name, salary, MAX(ord_date)
FROM (employee_tbl e JOIN employee_pay_tbl p ON e.emp_id = p.emp_id) JOIN orders_tbl o
ON e.emp_id = o.sales_rep
GROUP BY last_name, first_name, salary, o.sales_rep;

-- 14
SELECT prod_desc
FROM products_tbl
WHERE cost > (SELECT AVG(cost)
              FROM products_tbl);
              
-- 15
SELECT last_name, first_name, salary, bonus, 
        (SELECT SUM(salary) FROM employee_pay_tbl) as "Salariu total",
        (SELECT SUM(bonus) FROM employee_pay_tbl) as "Bonus total"
FROM employee_tbl e, employee_pay_tbl p
WHERE e.emp_id = p.emp_id;

-- 16
SELECT DISTINCT city
FROM employee_Tbl e
WHERE (SELECT COUNT(1)
       FROM orders_tbl
       WHERE sales_rep = e.emp_id) >=
      (SELECT MAX(COUNT(1))
       FROM orders_tbl
       GROUP BY sales_rep);
       
-- 17

SELECT emp_id, last_name, COUNT(DECODE(TO_CHAR(ord_date, 'mm'), 9, 1)) "Sept",
                          COUNT(DECODE(TO_CHAR(ord_date, 'mm'), 10, 1)) "Oct"
FROM employee_tbl e LEFT JOIN orders_tbl o
ON emp_id = sales_rep
GROUP BY emp_id, last_name;

-- 18

SELECT cust_name, cust_city
FROM customer_tbl c
WHERE cust_id NOT IN (SELECT cust_id FROM orders_tbl) AND REGEXP_LIKE(cust_address, '^[0-9]');

-- 19

SELECT DISTINCT e.emp_id, last_name, city, c.cust_id, cust_name, cust_city
FROM employee_tbl e, customer_tbl c
WHERE EXISTS (SELECT 1
              FROM orders_tbl
              WHERE sales_rep = e.emp_id AND cust_id = c.cust_id)
             AND city != cust_city;

-- 20

SELECT AVG(NVL(salary, 0)) as "Average salary"
FROM employee_pay_tbl;

-- 21

SELECT CUST_ID, CUST_NAME  ---- DA
FROM CUSTOMER_TBL
WHERE CUST_ID =
(SELECT CUST_ID
FROM ORDERS_TBL
WHERE ORD_NUM = '16C17');

SELECT EMP_ID, SALARY  ---- NU
FROM EMPLOYEE_PAY_TBL
WHERE SALARY BETWEEN '20000'
AND (SELECT SALARY
FROM EMPLOYEE_ID
WHERE SALARY = '40000');


-- 22

SELECT last_name, pay_rate
FROM employee_tbl e, employee_pay_tbl p
WHERE e.emp_id = p.emp_id
AND pay_rate > (SELECT MAX(pay_rate)
                FROM employee_tbl e2, employee_pay_tbl p2
                WHERE e2.emp_id = p2.emp_id
                AND UPPER(last_name) like '%LL%');