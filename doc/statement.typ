#import "template.typ": *

#show: project.with(
  title: "Atividade 3 - Org. Arg. 1",
  authors: (
	"Profa. Dra. Cíntia Borges Margi (cintia@usp.br)",
	"Guilherme S. Salustiano (salustiano@usp.br)",
  ),
)
#show link: underline

= Parte 1 - Emulação de ponto flutuante
== Contexto
Podem ser encontrado no mercado diversos processares sem unidade de ponto flutuante,
seja por uma questão de custo, área ou limite energético.
Entretanto o ´float´ e ´double´ são definido no C, assim sendo os códigos contendo 
esse tipo precisam rodas em todas as arquiteturas. Como isso é possivel?

Para investigar vamos tentar compilar o seguindo código para uma arquitetura sem ponto flutuante,
no caso do RISC-V isso é representado pela ausência da letra `f` e `d` da ISA,
por se tratar de uma ISA modular ela tem a base de inteiros (como rv32i ou rv64i),
e cada letra representa um novo conjunto de intruções @wiki:RISC-V.

Para isso vamos usar o #link("https://godbolt.org/")[godbolt], 
com o compilador `RISC-V (64-bits) gcc` com as flags `-O2 -march=rv64i -mabi=lp64`,
em `-march` definimos a arquitetura alvo como um processador RISC-V sem nunhuma extensão
e em `-mabi` definimos a interface binária para também não usar ponto flutuante.

Agora digitando o seguinte código:
```c
float sum(float x, float y) {
    return x + y;
}
```

#link("https://godbolt.org/z/Ej4MP1jne")[Observamos a seguinte saída]:
```asm
sum(float, float):
        mv      a5,a1
        mv      a1,a0
        addi    sp,sp,-16
        mv      a0,a5
        sd      ra,8(sp)
        call    __addsf3
        ld      ra,8(sp)
        addi    sp,sp,16
        jr      ra
```
Conforme esperado o processador não realiza a soma, 
e delegou a função `__addsf3` que recebe `y` e `x` 
nos registradores `a0` e `a1` (observe que ele precisa
alterar os parametros, uma vez que a soma de pontos flutuantes
#link("https://stackoverflow.com/a/24446382")[não é cumutativa]).

A função `__addsf3` faz parte da biblioteca de runtime de C,
que permite a portabilidade da linguagem em diversos sistemas.
As outras rotinas podem ser encontradas
#link("https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html")[aqui].

## Tarefa
Impremente uma biblioteca de ponto flutuante



= Entrega final
Ao final, gere um zip `atv2.zip` com os arquivos.
```bash
atv2.zip
├── c_O0.csv
├── c_O2.csv
├── java.csv
├── pypy.csv
├── python.csv
```

#bibliography("references.bib")
