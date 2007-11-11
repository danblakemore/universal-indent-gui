lupdate src -ts .\translations\universalindent_de.ts
lupdate src -ts .\translations\universalindent_zh_TW.ts
lupdate src -ts .\translations\universalindent_ja_JP.ts
lupdate src -ts .\translations\universalindent.ts
linguist .\translations\universalindent_de.ts
lrelease .\translations\universalindent_de.ts -qm .\translations\universalindent_de.qm
pause
