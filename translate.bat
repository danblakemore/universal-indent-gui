lupdate src -ts .\translations\universalindent_de.ts
lupdate src -ts .\translations\universalindent_tw.ts
lupdate src -ts .\translations\universalindent_ja.ts
lupdate src -ts .\translations\universalindent.ts
linguist .\translations\universalindent_de.ts
lrelease .\translations\universalindent_de.ts -qm .\translations\universalindent_de.qm
pause
