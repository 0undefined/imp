# ECS centric language

Utilize an ECS engine, possibly compiled, dynamic would be cool too.
Strongly Typed
Functional
Generic Systems? -- instantiate types of systems compile time.


# System definition
```
System A ((v,p) : (Vel, Pos)) {
  p += v;
}
```
or
```
System A (e : (Vel, Pos)) {
  e.p += e.v;
}
```

With predicates:
```
~some uniform variable c~
System A ((v,p) : (Vel, Pos)) ? p > 0 && p < c {
  p += v;
}
```
Will make the engine perform immediate predicate check of `p > 0 && p < c` after
each entity modifying system has executed, on each entity that satisfies the
pattern (Vel, Pos).

Language wise:
```futhark
type Predicate = (\e -> bool)
System : (\e -> e) ([]Predicate)
map (\f -> if ! any pred f then f else sys f) e
```

typedef uint64_t entity_t;
typedef entity_id_t entity_t;
typedef ((bool)(entity_t)) predicate_t;

typedef ((void)entity_t) system_f;
typedef (struct {*predicate_t; int; }) system_t;

Chain the order of the systems together:

map A e |> map B |> map C


# Execution model
use double buffering?

```
e[2][NUM_E];
for (e : {Vel,Pos} in E) do e' = A e
```
or
```
e[NUM_E];
for (e : {Vel,Pos} in E) do e = A e
```


