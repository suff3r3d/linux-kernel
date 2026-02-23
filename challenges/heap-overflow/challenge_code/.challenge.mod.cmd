savedcmd_challenge.mod := printf '%s\n'   challenge.o | awk '!x[$$0]++ { print("./"$$0) }' > challenge.mod
