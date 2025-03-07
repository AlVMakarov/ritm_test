Решение тестового задания.  

Для запуска:  
$ make  
$ ./main indata outdata  

От себя добавил тест "testdata_1". Агент-функция * в моем коде работает для произвольного числа смежных ребер над вершиной, в тесте рассмотрен данный случай.  

Задание:  
Написать программу, которая для заданного аннотируемого метаграфа и агент-функции над ним выполнит вычисление атрибутов всех элементов.    

Вход программы: файл описанного ниже формата.   

1. первая строка – пара чисел: количество узлов (NV) и рёбер (NE) графа;
2. пустая строка;
3. NE пар номеров узлов, соответствующих началу и концу ребра;
4. пустая строка;
5. NV + NE правил агент-функции (сначала для узлов, потом для рёбер).

Варианты правил:    
1. одно число – значение атрибута.  
2. буква и число – значение атрибута копируется из указанного ребра (e) или узла (v).   
3. имя функции – вычисление функции:   
    min – определена только для узлов; вычисляет минимальное значение атрибутов рёбер, входящих в узел.   
    * – определена только для рёбер; вычисляет произведение значений атрибутов левого узла и входящих в него рёбер.   

Результат работы программы: файл со списком значений атрибутов элементов графа, сначала узлов, затем рёбер.
Ожидаемый результат: исходный код программы.
Программа должна принимать на вход имя файла с задачей и имя файла для записи результата.

Определения:
Аннотируемый метаграф — ориентированный граф, каждый элемент которого описывается набором атрибутов.
Агент-функция — набор правил преобразования графа и его элементов.

Пример:
Агент-функция:                                                   
vertex[1].atr = 0.1     
vertex[2].atr = 2     
vertex[3].atr = 0.2     
vertex[4].atr = min(edge[b].atr, edge[c].atr)      
vertex[5].atr = min(edge[d].atr, edge[e].atr)      
vertex[6].atr = 0.5      
vertex[7].atr = 0.3      
vertex[8].atr = edge[h].atr      
edge[a].atr = vertex[1].atr      
edge[b].atr = vertex[2].atr * edge[a].atr   
edge[c].atr = vertex[3].atr  
edge[d].atr = vertex[4].atr   
edge[e].atr = vertex[6].atr * edge[g].atr   
edge[f].atr = vertex[5].atr   
edge[g].atr = vertex[7].atr   
edge[h].atr = vertex[7].atr   
   
Входной файл:   
8 8     # NV NE   
   
1 2     # список рёбер   
2 4        
3 4        
4 5        
6 5        
5 7        
7 6        
7 8        
     
0.1     # список правил       
2       
0.2       
min       
min       
0.5       
0.3       
e 8       
v 1       
*       
v 3       
v 4       
*       
v 5       
v 7       
v 7       
       
Ожидаемый результат:       
0.1       
2       
0.2       
0.2       
0.15       
0.5       
0.3       
0.3       
0.1       
0.2       
0.2       
0.2       
0.15       
0.15       
0.3       
0.3       


