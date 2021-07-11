import tornado.web
import json
import hashlib
import time
import os
from stat import *

def gen_hash(password):
    sha = hashlib.sha256()
    sha.update(password.encode("utf-8"))
    return sha.hexdigest()

def authenticated(request_body):
    try:
        data = json.loads(request_body)
        if not data["Token"] == webcontroller.token:
            return False
        else:
            return True
    except Exception as ex:
        pass
    return False

class Index(tornado.web.RequestHandler):
    def get(self):
        webcontroller.render_page(self, "index.html", { "LastLogin": webcontroller.auth_info["LastLogin"], "FailedLoginAttempts": webcontroller.auth_info["FailedLoginAttempts"] })

class DisplayCode(tornado.web.RequestHandler):
    def post(self):
        webcontroller.serialcontroller.display_code()

class Welcome(tornado.web.RequestHandler):
    def post(self):
        response = {}
        try:
            data = json.loads(self.request.body)
            if data["AuthCode"] == str(webcontroller.authCode):
                print "Welcome screen: Valid auth code received"
                webcontroller.auth_info["Password"] = gen_hash(data["Password"])
                webcontroller.auth_info["LastLogin"] = time.strftime("%Y-%m-%d") + " " + time.strftime("%H:%M:%S")            
                webcontroller.save_auth_info()
                webcontroller.serialcontroller.display_main_screen()            
                response = { "IsValidAuthCode": True, "IsValidPassword": True, "Token": webcontroller.token, "FailedLoginAttempts": webcontroller.auth_info["FailedLoginAttempts"] }
            else:
                print "Welcome screen: Invalid auth code received"
                response = { "IsValidAuthCode": False }
        except Exception as ex:
            print "Welcome screen ERROR:"
            print ex
            response = { "ErrorOccurred": True }
            pass
        self.write(json.dumps(response))

class Login(tornado.web.RequestHandler):
    def post(self):
        response = {}
        try:
            data = json.loads(self.request.body)
            if data["AuthCode"] == str(webcontroller.auth_code):
                print "Login screen: Valid auth code received"
                if gen_hash(data["Password"]) == webcontroller.auth_info["Password"]:
                    print "Login screen: Valid password received"
                    response = { "IsValidAuthCode": True, "IsValidPassword": True, "Token": webcontroller.token, "FailedLoginAttempts": webcontroller.auth_info["FailedLoginAttempts"] }
                    webcontroller.update_auth_info(True)
                    webcontroller.serialcontroller.display_main_screen()            
                else:
                    print "Login screen: Invalid password received"
                    webcontroller.update_auth_info(False)
                    response = { "IsValidAuthCode": True, "IsValidPassword": False }
            else:
                print "Login screen: Invalid auth code received"
                webcontroller.update_auth_info(False)
                response = { "IsValidAuthCode": False }
        except Exception as ex:
            print "Login screen ERROR:"
            print ex
            response = { "ErrorOccurred": True }
            pass
        self.write(json.dumps(response))

class AuthInfo(tornado.web.RequestHandler):
    def post(self):
        if not authenticated(self.request.body):
            return
        response = { "LastLogin": webcontroller.auth_info["LastLogin"], "FailedLoginAttempts": webcontroller.auth_info["FailedLoginAttempts"] }
        self.write(json.dumps(response)) 

class GetFiles(tornado.web.RequestHandler):
    def post(self):
        if not authenticated(self.request.body):
            return
        files = []
        safe_path = os.path.join(os.path.dirname(__file__), "../../data/safe/")
        for fn in os.listdir(safe_path):
            file_info = os.stat(safe_path + fn)
            f = {}
            f["FileName"] = fn
            f["DateAdded"] = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(file_info[ST_CTIME]))
            f["Size"] = file_info[ST_SIZE]
            files.append(f)
        self.write(json.dumps(files))

class Upload(tornado.web.RequestHandler):
    def post(self):
        token = self.get_argument("Token", default=None, strip=False)
        if token is None or token != webcontroller.token: 
            return
        file_info = self.request.files["file"][0]
        file_name = file_info["filename"]
        uploads_path = os.path.join(os.path.dirname(__file__), "../../data/upload/")
        file = open(uploads_path + file_name, "wb")
        file.write(file_info["body"])
        file.close()


