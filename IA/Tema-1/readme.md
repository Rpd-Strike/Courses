## Descriere euristici

### 1. Estimarea inadmisibila

```python
def inadmissibleEst(node: StateNode) -> float:
    return len(node.state[0]) * 1000
```

Un exemplu pentru care aceasta estimare este proasta este:
```
aa
aa
```

si dorim sa obtinem doar `a`. Euristica va estima `2000` dar putem ajunge la destinatie cu costul `2`

### 2. Estimarea banala

```python
def basicEst(node: StateNode) -> float:
    return 1
```

Aceasta estimare este corecta, intrucat orice operatie are cel putin costul `1`

### 3. Estimarea bazata pe coloane

```python
def columnsNeededEst(node: StateNode) -> float:
    return len(node.state[1]) - len(getFinalNode().state[1])
```

Aceasta estimare se bazeaza pe numarul de coloane pe care trebuie neaparat sa le eliminam pentru a ajunge la un numar de coloane egal cu cel din starea finala

### 3. Estimarea bazata pe coloane si linii

```python
def smartEst(node: StateNode) -> float:
    col_cost = len(node.state[1]) - len(getFinalNode().state[1])
    lin_cost = len(getFinalNode().state[1])
    return col_cost + lin_cost
```

Aceasta estimare se bazeaza atat pe cate coloane trebuie sa eliminam, dar si pe cate linii trebuie sa eliminam. Cum atunci cand eliminam linii, costul este mai mare pe masura cu cate coloane avem, mai intai vom considera ca eliminam coloane, ignorand costul dat de elementele adiacente diferite, pentru ca atunci cand eliminam liniile sa avem un cost minim.

## Tabele

### Testul `exemplu.test`

| **Algoritm / Estimare** | **Lungime** | **Cost** | **Vizitate** | **Calculate** | **Timp** |
| -- | -- | -- | -- | -- | -- |
| UCS | 6 | 14 | 1560 | 4173 | 0.79 s |
| A* / Bad | 5 | 15 | 90 | 145 | 0.01 s |
| A* / Basic | 6 | 14 | 1560 | 4173 | 0.88 s |
| A* / Columns | 6 | 14 | 1079 | 3178 | 0.93 s |
| A* / Smart | 6 | 14 | 1079 | 3178 | 1.22 s |
| A* Fast / Bad | 5 | 15 | 18 | 37 | 0.01 s |
| A* Fast / Basic | 6 | 14 | 15 | 148 | 0.36 s |
| A* Fast / Columns | 6 | 14 | 24 | 148 | 0.35 s |
| A* Fast / Smart | 6 | 14 | 24 | 148 | 0.41 s |
| IDA* / Bad | 7 | 21 | 117 | 117 | 0.02 s |
| IDA* / Basic | 6 | 14 | 11334 | 11334 | 0.73 s |
| IDA* / Columns | 6 | 14 | 6161 | 6161 | 0.40 s |
| IDA* / Smart | 6 | 14 | 1476 | 6161 | 0.46 s |

-----------

### Testul `verybig.test`

| **Algoritm / Estimare** | **Lungime** | **Cost** | **Vizitate** | **Calculate** | **Timp** |
| -- | -- | -- | -- | -- | -- |
| UCS | TO | TO | TO | TO | TO |
| A* / Bad | 6 | 13 | 691 | 1160 | 0.23 s |
| A* / Basic | TO | TO | TO | TO | TO |
| A* / Columns | TO | TO | TO | TO | TO |
| A* / Smart | TO | TO | TO | TO | TO |
| A* Fast / Bad | 6 | 13 | 29 | 149 | 0.18 s |
| A* Fast / Basic | 7 | 12 | 151 | 853 | 12.52 s |
| A* Fast / Columns | 7 | 12 | 234 | 853 | 10.22 s |
| A* Fast / Smart | 7 | 12 | 234 | 853 | 10.27 s |
| IDA* / Bad | 8 | 19 | 48 | 48 | 0.009 s |
| IDA* / Basic | 7 | 12 | 165717 | 165717 | 6.81 s |
| IDA* / Columns | 7 | 12 | 104416 | 104416 | 4.65 s |
| IDA* / Smart | 7 | 12 | 23662 | 104416 | 4.89 s |

Legenda: TO -->> Timeout

Observam ca A* Fast este cel mai rapid in general, intrucat nu repeta stari. 
In acelasi timp, pe exemplul mare, IDA* este mai rapid intrucat se duce mai repede pe ramura buna.
Evident, UCS este cel mai lent, intrucat este un algoritm exponential
