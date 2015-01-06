#include "sqlkeywords.h"

const std::string SQLGenerator::WILDCARD = "* ";

std::string SQLGenerator::prepareStatement(std::string table, std::string columns, std::string criteria, std::string pattern, std::string pattern2, size_t statementType)
{
    std::string query;
    /*Let's make sure each field/ parameter has a space at the end. SQL databases are sensitive to white space!*/
    if(table[table.size()-1] != ' ')
    {
        table += " ";
    }
    if(columns[columns.size()-1] != ' ')
    {
        columns += " ";
    }
    if(criteria[criteria.size()-1] != ' ')
    {
        criteria += " ";
    }
    if(pattern[pattern.size()-1] != ' ')
    {
        pattern += " ";
    }
    if(pattern2[pattern2.size()-1] != ' ')
    {
        pattern2 += " ";
    }
    /*With that check out of the way, we can start producing the query!*/
    switch(statementType)
    {
    /*SELECT column_name,column_name FROM table_name; */
    case SELECT:
    case SELECT | FROM:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table;
        break;
    /*CREATE DATABASE dbname;*/
    case CREATEDATABASE:
        query = Keywords[CREATEDATABASE] + table;
        break;
    /*CREATE TABLE table_name
    (
    column_name1 data_type(size),
    column_name2 data_type(size),
    column_name3 data_type(size),
    ....
    );*/
    case CREATETABLE:
        query = Keywords[CREATETABLE] + table;
        break;
    /*SELECT column_name,column_name
    FROM table_name
    WHERE column_name operator value;
    */
    case SELECT | WHERE:
    case SELECT | FROM | WHERE:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[WHERE] + criteria;
        break;
    /*SELECT * FROM Customers
    WHERE Country='Germany'
    AND City='Berlin';
    */
    case SELECT | AND:
    case SELECT | FROM | AND:
    case SELECT | FROM | WHERE | AND:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[WHERE] + criteria + Keywords[AND] + pattern;
        break;
    /*SELECT * FROM Customers
    WHERE City='Berlin'
    OR City='München';
    */
    case SELECT | OR:
    case SELECT | FROM | OR:
    case SELECT | FROM | WHERE | OR:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[WHERE] + criteria + Keywords[OR] + pattern;
        break;
    /*SELECT * FROM Customers
    WHERE Country='Germany'
    AND (City='Berlin' OR City='München');
    */
    case SELECT | AND | OR:
    case SELECT | FROM | AND | OR:
    case SELECT | FROM | WHERE | AND | OR:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[WHERE] + criteria + Keywords[AND] + "(" + pattern +
            Keywords[OR] + pattern2 + ")";
        break;
    /*SELECT column_name,column_name
    FROM table_name
    ORDER BY column_name,column_name ASC|DESC;
    */
    case ORDERBY:
    case SELECT | ORDERBY:
    case SELECT | FROM | ORDERBY:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[ORDERBY] + criteria;
        break;
    /*INSERT INTO table_name
    VALUES (value1,value2,value3,...);
    or
    INSERT INTO table_name (column1,column2,column3,...)
    VALUES (value1,value2,value3,...);
    */
    case INSERT:
    case INSERT | INTO:
    case INSERT | INTO | VALUES:
        query = Keywords[INSERT] + Keywords[INTO] + table + "(" + columns + ")" + Keywords[VALUES] + "(" + criteria + ")";
        break;
    /*UPDATE table_name
    SET column1=value1,column2=value2,...
    WHERE some_column=some_value;
    */
    case UPDATE:
    case UPDATE | SET:
    case UPDATE | SET | WHERE:
        query = Keywords[UPDATE] + table + Keywords[SET] + columns + Keywords[WHERE] + criteria;
        break;
    /*DELETE FROM table_name
    WHERE some_column=some_value;
    */
    case DELETE:
    case DELETE | FROM:
    case DELETE | FROM | WHERE:
        query = Keywords[DELETE] + Keywords[FROM] + table + Keywords[WHERE] + criteria;
        break;
    /*SELECT column_name(s)
    FROM table_name
    WHERE column_name LIKE pattern;
    */
    case LIKE:
    case SELECT | FROM | WHERE | LIKE:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[WHERE] + criteria + Keywords[LIKE]
            + pattern;
        break;
    /*SELECT column_name(s)
    FROM table_name
    WHERE column_name IN (value1,value2,...);
    */
    case IN:
    case SELECT | FROM | WHERE | IN:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[WHERE] + criteria +
            Keywords[IN] + pattern;
        break;
    /*SELECT column_name(s)
    FROM table_name
    WHERE column_name BETWEEN value1 AND value2;
    */
    case BETWEEN:
    case BETWEEN | AND:
    case SELECT | FROM | WHERE | BETWEEN | AND:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[WHERE] + criteria +
            Keywords[BETWEEN] + pattern + Keywords[AND] + pattern2;
        break;
    /*SELECT column_name AS alias_name
    FROM table_name;
    */
    case AS:
    case SELECT | AS:
    case SELECT | AS | FROM:
        query = Keywords[SELECT] + columns + Keywords[AS] + criteria + Keywords[FROM] + table;
        break;
    /*SELECT column_name(s)
    FROM table_name AS alias_name;
    */
    case SELECT | FROM | AS_TABLE:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[AS] + criteria;
        break;
    /*SELECT Orders.OrderID, Customers.CustomerName, Orders.OrderDate
    FROM Orders
    INNER JOIN Customers
    ON Orders.CustomerID=Customers.CustomerID;
    or
    SELECT column_name(s)
    FROM table1
    JOIN table2
    ON table1.column_name=table2.column_name;
    */
    case INNER_JOIN:
    case INNER_JOIN | ON:
    case SELECT | FROM | INNER_JOIN:
    case SELECT | FROM | INNER_JOIN | ON:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[INNER_JOIN] + criteria
            + Keywords[ON] + pattern;
        break;
    /*SELECT column_name(s)
    FROM table1
    LEFT JOIN table2
    ON table1.column_name=table2.column_name;
    or
    SELECT column_name(s)
    FROM table1
    LEFT OUTER JOIN table2
    ON table1.column_name=table2.column_name;
    */
    case LEFT_JOIN:
    case SELECT | FROM | LEFT_JOIN:
    case SELECT | FROM | LEFT_JOIN | ON:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[LEFT_JOIN] + criteria
            + Keywords[ON] + pattern;
        break;
    //Special case of above
    /*SELECT Customers.CustomerName, Orders.OrderID
    FROM Customers
    LEFT JOIN Orders
    ON Customers.CustomerID=Orders.CustomerID
    ORDER BY Customers.CustomerName;
    */
    case LEFT_JOIN | ORDERBY:
    case LEFT_JOIN | ON | ORDERBY:
    case SELECT | FROM | LEFT_JOIN | ON | ORDERBY:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[LEFT_JOIN] + criteria
            + Keywords[ON] + pattern + Keywords[ORDERBY] + pattern2;
        break;
    /*SELECT column_name(s)
    FROM table1
    RIGHT JOIN table2
    ON table1.column_name=table2.column_name;
    or
    SELECT column_name(s)
    FROM table1
    RIGHT OUTER JOIN table2
    ON table1.column_name=table2.column_name;
    */
    case RIGHT_JOIN:
    case SELECT | RIGHT_JOIN:
    case SELECT | FROM | RIGHT_JOIN:
    case SELECT | FROM | RIGHT_JOIN | ON:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[RIGHT_JOIN] + criteria +
            Keywords[ON] + pattern;
        break;
    /*SELECT column_name(s)
    FROM table1
    FULL OUTER JOIN table2
    ON table1.column_name=table2.column_name;
    */
    case FULL_JOIN:
    case SELECT | FULL_JOIN:
    case SELECT | FROM | FULL_JOIN:
    case SELECT | FROM | FULL_JOIN | ON:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[FULL_JOIN] + criteria +
            Keywords[ON] + pattern;
        break;
    /*SELECT column_name(s)
    INTO newtable [IN externaldb]
    FROM table1;
    */
    //The [IN externaldb] has to be inserted into the criteria string parameter by the user.
    case SELECT | INTO:
    case SELECT | INTO | FROM:
        query = Keywords[SELECT] + columns + Keywords[INTO] + criteria + Keywords[FROM] + table;
        break;
    /*INSERT INTO table2
    (column_name(s))
    SELECT column_name(s)
    FROM table1;
    */
    case INTO_SELECT:
    case INTO_SELECT | INSERT | FROM:
    case INSERT | SELECT | INTO:
        query = Keywords[INSERT] + Keywords[INTO] + criteria + Keywords[SELECT] + columns + Keywords[FROM]
            + table;
        break;
    /*SELECT column_name(s) FROM table1
    UNION
    SELECT column_name(s) FROM table2;
    or
    SELECT column_name(s) FROM table1
    UNION ALL
    SELECT column_name(s) FROM table2;
    */
    case UNION:
    case SELECT | UNION:
    case SELECT | FROM | UNION:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[UNION] +
            Keywords[SELECT] + columns + Keywords[FROM] + criteria;
        break;
    case UNION | ALL:
    case SELECT | UNION | ALL:
    case SELECT | FROM | UNION | ALL:
        query = Keywords[SELECT] + columns + Keywords[FROM] + table + Keywords[UNION] + Keywords[ALL] +
            Keywords[SELECT] + columns + Keywords[FROM] + criteria;
        break;
    default:
        std::cout << "An error occured with the SQL query generator! The user passed a switch "
            << "combination to statementType that is invalid and / or not implemented yet!"
            << std::endl;
    }
    query += ";";//query terminating character!
    return query;
}
