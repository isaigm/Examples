% Caso base: 1 es un número impar.
punto(0, 1).
par(s(s(0))).

par(s(s(X))) :- write(X),write('\n'), par(X) .

suma(A, B, C) :- C is A + B.
lista(M, N) :- M<N, nl, write(M), NuevoM is M + 1, lista(NuevoM, N).

hola :- write('hola mundo').

sobre(a, b).
sobre(d, c).
sobre(e, d).
libre(a).
libre(e).
ocupado(X) :- sobre(Y, X).
base(X, Y) :- sobre(Y, X).
