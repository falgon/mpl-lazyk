# mpl-lazyk

![CI](https://github.com/falgon/mpl-lazyk/workflows/CI/badge.svg)

> _"Use template metaprogramming only when you really need to"<br>
> ― [C++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rt-metameta)_

mpl-lazyk is a monadic and impractical compile-time lazyk interpreter with template metaprogramming.
This is achieved by implementing a simple parser combinator consisting of a monad transformer and either monad.
The implementation and naming of many of these algebraic data structures is influenced by the programming language Haskell and category theory.

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
$ INPUT="Que Sera, Sera" LAZYK_CODE="****i*i*i*ii*ii*ii11111110001111111110000011111111100000" docker-compose -f docker/docker-compose.yml run lazyk
Que Sera, Sera
```

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

## FYI

- There is [my blog post](https://roki.dev/roki.log/2020/12/16/CompileTimeLazyKWithCXXTemplateMetaProgramming/index.html) about  this implementation (Japanese)
- If you enojoy this, write your impressions on [reddit](https://www.reddit.com/r/cpp/comments/kt72dz/lazy_k_interpreter_in_c_template_metaprogramming/)!
