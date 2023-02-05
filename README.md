# expression

Cによる再帰下降型の式の評価の一般的なサンプルプログラムです。
コンパイラやインタプリタを書くときに役に立つと思います。

doubleで演算しています。括弧（）が使えます。演算子は四則演算(+,-,*,/)です。
evaluate_expression()がエントリです。

グローバル変数を使っていることと、readc()で、一文字先読みして各関数を呼んでいるので、将来的にはそれらを書き直す必要があります。書き直したら、もっと優秀な関数になると思います。

アルゴリズムは、「C言語による標準アルゴリズム事典」奥村晴彦著によります。

実行し、式を入力すると、式を評価し、値を返します。
