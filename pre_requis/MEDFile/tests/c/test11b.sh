file=test11b
output=/dev/null && test "x${OUTPUT}" = "xyes" && output=${file}.out
${CHECKER} ${EXECDIR}/${file} > ${output} 2>&1
