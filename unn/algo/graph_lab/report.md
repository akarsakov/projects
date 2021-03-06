����� ���������� ����� � �����
========================================================

� ������ ������ ����������� ��������� ��������� ��� ������ ���������� ����� � �����:
+ [�������� ��������][1]
+ [�������� ��������-�����][2]

� ������ ������ ���������� �������� �������� � �������������� �������� ����, ����� �������, ��� ��������� ����������: O((E+V)*log(V)), 
��� V - ���������� ������ � �����, E - ���������� ����� � �����. � ���� ������� �������� ��������-����� ����� ��������� O(V*E).

�������� ��������� ������������� ��� ����, ����� �������� ��� ���������� ���� ���������� ������������ ���������� ������������� ������.


```r
library(ggplot2)
```

```
## Warning: package 'ggplot2' was built under R version 3.1.1
```

```r
Dijkstra.exp1 = read.csv("data/1_Dejkstra_a.csv")
Dijkstra.exp2 = read.csv("data/1_Dejkstra_b.csv")
FordBellman.exp1 = read.csv("data/1_FordBellman_a.csv")
FordBellman.exp2 = read.csv("data/1_FordBellman_b.csv")
exp1 <- data.frame(Dijkstra.exp1$n, Dijkstra.exp1$time, Dijkstra.exp2$time, FordBellman.exp1$time, FordBellman.exp2$time)
colnames(exp1) <- c("n", "y1", "y2", "y3", "y4")
ggplot(exp1, aes(n, y = Time (ms), color = Algorithm)) + 
    geom_smooth(aes(y = y1, col="Dijkstra (m=n^2/10)"), method=loess) + 
    geom_smooth(aes(y = y2, col="Dijkstra (full graph)"), method=loess) +
    geom_smooth(aes(y = y3, col="Ford-Bellman (m=n^2/10)"), method=loess) + 
    geom_smooth(aes(y = y4, col="Ford-Bellman (full graph)"), method=loess)
```

![plot of chunk unnamed-chunk-1](figure/unnamed-chunk-1-1.png) 

�������, ��� ��������� ��������� ������������� O((E+V)*log(V)). �������� ����� �� (n^2 + n)*log(n). � ������� ����������� ������ ������:


```r
Dijkstra.exp1$time = Dijkstra.exp1$time / ((Dijkstra.exp1$n * Dijkstra.exp1$n + Dijkstra.exp1$n) * log2(Dijkstra.exp1$n))
ggplot(Dijkstra.exp1, aes(n, y = ratio)) + geom_line(aes(y = time), method=loess)
```

![plot of chunk unnamed-chunk-2](figure/unnamed-chunk-2-1.png) 

�������� ����������� ������ ��� ��������� ��������-�����:


```r
FordBellman.exp2$n = FordBellman.exp2$n / 10
FordBellman.exp2$time = FordBellman.exp2$time / (FordBellman.exp2$n * FordBellman.exp2$n * FordBellman.exp2$n)
ggplot(FordBellman.exp2, aes(n, y = ratio)) + geom_line(aes(y = time), method=loess)
```

![plot of chunk unnamed-chunk-3](figure/unnamed-chunk-3-1.png) 

������ ������� ���������� ����� ��������������� O(n):


```r
Dijkstra.exp3 = read.csv("data/2_Dejkstra_a.csv")
Dijkstra.exp4 = read.csv("data/2_Dejkstra_b.csv")
FordBellman.exp3 = read.csv("data/2_FordBellman_a.csv")
FordBellman.exp4 = read.csv("data/2_FordBellman_b.csv")
exp2 <- data.frame(Dijkstra.exp3$n, Dijkstra.exp3$time, Dijkstra.exp4$time, FordBellman.exp3$time, FordBellman.exp4$time)
colnames(exp2) <- c("n", "y1", "y2", "y3", "y4")
ggplot(exp2, aes(n, y = Time (ms), color = Algorithm)) + 
    geom_smooth(aes(y = y1, col="Dijkstra (m=100*n)"), method=loess) + 
    geom_smooth(aes(y = y2, col="Dijkstra (m=1000*n)"), method=loess) +
    geom_smooth(aes(y = y3, col="Ford-Bellman (m=100*n)"), method=loess) + 
    geom_smooth(aes(y = y4, col="Ford-Bellman (m=1000*n)"), method=loess)
```

![plot of chunk unnamed-chunk-4](figure/unnamed-chunk-4-1.png) 

```r
Dijkstra.exp3$time = Dijkstra.exp3$time / (101*Dijkstra.exp3$n * log2(Dijkstra.exp3$n))
ggplot(Dijkstra.exp3, aes(n, y = ratio)) + geom_line(aes(y = time), method = loess)
```

![plot of chunk unnamed-chunk-4](figure/unnamed-chunk-4-2.png) 

```r
FordBellman.exp3$n = FordBellman.exp3$n / 10
FordBellman.exp3$time = FordBellman.exp3$time / (FordBellman.exp3$n * FordBellman.exp3$n)
ggplot(FordBellman.exp3, aes(n, y = ratio)) + geom_line(aes(y = time), method=loess)
```

![plot of chunk unnamed-chunk-4](figure/unnamed-chunk-4-3.png) 

�������� �����-�������� ����� ���� ������ ���������� ��������, ��� �������� ��������. �������� ��� �����������, ����� ���������� ����� � ����� ��������������� �������� ���������� ������. ����� ������� ��� �������� ������ �������������� ������������ �������� ��������. � ������ �������, �������� �������� �������� �����������, ���� � ����� ���� ������������� ����.

[1]: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
[2]: https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
