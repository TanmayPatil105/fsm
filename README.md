# fsm - Finite State Machine

Takes regex as input and then creates an automate to handle user input.

### Example

```bash
$ ./fsm "(a|b)*|((ab)d|(c+))"
S -> G
K -> AK
J -> I | K
I -> HB
H -> DC
G -> F | J
F -> EF | ε
E -> D | C
D -> a
C -> b
B -> d
A -> c
```

TODO:
- Convert to Chomsky Normal Form
- Check user string with regex
