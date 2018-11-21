壓縮檔內包含Makefile make即可編譯

程式步驟
// input
// infix to postfix
// evaluate postfix
// print

對於 unary + & -的處理： 我是把他們和前後的 + & - 一起分析，
用負負得正，正負得負的方式去得到一個binary + or -，
如果一個infix的式子最前面有unary + or - ，此方法會失敗