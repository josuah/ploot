#!/bin/sh
awk='
BEGIN {
	tab = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
	print "/**/"
}

END {
	print ""
	print "#endif"
}

# functions

args {
	sub(/^[ \t]*/, " ")
	args = args $0
}

/^[a-zA-Z0-9_]+\([][)(a-z_A-Z0-9*,. \t]*$/ {
	if (match(type, "static") || match($0, ";$"))
		next

	symbol = $0
	sub(/\(.*/, "", symbol)
	sub(/[a-zA-Z0-9_]*\(/, "", $0)
	if (symbol == "main")
		next

	args = $0
	sub(/^[a-z]*\(/, "", args)
}

args && /\)$/ {
	gsub(/[\n \t]+/, " ", args)

	sub(/\)$/, "", args)

	gsub(/[a-zA-Z0-9_]+\[[^]]*\]/, "[]", args)
	gsub(/[*][a-zA-Z0-9_]+/, "*", args)
	gsub(/[ ][a-zA-Z0-9_]+,/, ",", args)
	gsub(/[ ][a-zA-Z0-9_]+$/, "", args)
	gsub(/[ ][a-zA-Z0-9_]+\*/, "*", args)
	gsub(/\.\.\.\$/, "...", args)
	gsub(/void\)$/, "void", args)

	printf("%s%s%s%s(%s);\n",
	    type, substr(tab, 1, 20 / 8 - (length(type) - 3) / 8),
	    symbol, substr(tab, 1, 30 / 8 - (length(symbol) - 1) / 8),
	    args)

	args = ""
}

!args {
	type = $0
}

# variables

/^[a-zA-Z][][ \t*a-z_A-Z0-9]*=.*[;{]$/ && $1 != "static" && $1 != "enum" {
	sub(/ *=.*/, ";")
	sub(/[ \t]*;$/, ";");
	print
}
'

for file in src/*.c; do file=${file%.c}
	grep -Fq '/**/' "$file.h" 2>/dev/null || continue
	header=$(awk '$0 == "/**/" { exit(0) } 1' "$file.h"
		awk "$awk" "$file.c")
	printf '%s\n' "$header" >"$file.h"
done
