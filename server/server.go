package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"regexp"

	"github.com/widuu/gojson"
)

func main() {
	http.HandleFunc("/", handler)
	http.ListenAndServe(":4399", nil)
}

func handler(w http.ResponseWriter, r *http.Request) {
	result0, _ := ioutil.ReadAll(r.Body)
	r.Body.Close()
	result := string(result0)
	mtype := gojson.Json(result).Get("message_type").Tostring()
	msg := gojson.Json(result).Get("message").Tostring()
	group := gojson.Json(result).Get("group_id").Tostring()
	user := gojson.Json(result).Get("user_id").Tostring()
	r1, _ := regexp.Compile(`(多少度|温度)`)
	match := r1.MatchString(msg)
	if match == true {
		if mtype == "group" {
			data := make(url.Values)
			data["group_id"] = []string{group}
			temp := readserial()
			log.Println(temp + "   " + user)
			data["message"] = []string{"现在温度是" + temp + "°"}
			res, err := http.PostForm("http://127.0.0.1:5700/send_group_msg", data)
			if err != nil {
				fmt.Println(err.Error())
				return
			}
			defer res.Body.Close()
		}
	}
}
