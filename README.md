# mpl-lazyk

![CI](https://github.com/falgon/mpl-lazyk/workflows/CI/badge.svg)

> _"Use template metaprogramming only when you really need to"<br>
> ― [C++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rt-metameta)_

mpl-lazyk is a compile-time LazyK interpreter with template metaprogramming.
This is achieved by implementing a simple parser combinator consisting of a monad transformer and either monad.
The implementation and naming of many of these algebraic data structures is influenced by the programming language Haskell and category theory.

## Tests

Build and run

```bash
$ cd tests
$ make
```

Or inside the docker container...

```bash
$ docker-compose -f docker/docker-compose.yml run test
```

## Run

Build and run

```bash
$ cd app
$ make LAZYK_CODE='"(SKK)I"' INPUT='"Hello, world!"'
Hello, world!
```

Or inside the docker container...

```bash
$ docker-compose -f docker/docker-compose.yml run lazyk
Hello, world!
$ INPUT="Que Sera, Sera" LAZYK_CODE="(SKK)I" docker-compose -f docker/docker-compose.yml run lazyk
Que Sera, Sera
```
