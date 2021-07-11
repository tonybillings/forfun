import os
import time
import tornado
import tornado.web
import tornado.ioloop
import tornado.httpserver
import uuid
import pickle
import httpHandlers

class WebController():
    def __init__(self):
        self.token = str(uuid.uuid1())
        self.auth_info = {}
        self.UPLOADS_PATH = "web/uploads/"
        self.IMAGES_PATH = "static/images"
        self.SCRIPTS_PATH = "static/scripts"
        self.STYLES_PATH = "static/styles"
        self.handlers = httpHandlers
        self.handlers.webcontroller = self

    def get_auth_info(self):
        file_path = os.path.join("./auth/", ".auth_info")
        if not os.path.isfile(file_path):
            self.auth_info["LastLogin"] = "Never"
            self.auth_info["FailedLoginAttempts"] = 0
            self.auth_info["Password"] = ""
            file = open(file_path, "wb")
            pickle.dump(self.auth_info, file)
            file.close()
        else:
            file = open(file_path, "rb")
            self.auth_info = pickle.load(file)
            file.close()

    def update_auth_info(self, success):
        if success == True:
            self.auth_info["LastLogin"] = time.strftime("%Y-%m-%d") + " " + time.strftime("%H:%M:%S") 
            self.auth_info["FailedLoginAttempts"] = 0
        else:
            count = self.auth_info["FailedLoginAttempts"] + 1
            self.auth_info["FailedLoginAttempts"] = count
        self.save_auth_info()

    def save_auth_info(self):
        file_path = os.path.join("./auth/", ".auth_info")
        file = open(file_path, "wb")
        pickle.dump(self.auth_info, file)
        file.close()

    def start(self):
        self.get_auth_info()
        handler_list = [
            (r"/", self.handlers.Index),
            (r"/display_code", self.handlers.DisplayCode),
            (r"/welcome", self.handlers.Welcome),
            (r"/login", self.handlers.Login),
            (r"/auth_info", self.handlers.AuthInfo),
            (r"/get_files", self.handlers.GetFiles),
            (r"/upload", self.handlers.Upload),
        ]
        settings = dict(
            handlers=handler_list,
            template_path=os.path.join(os.path.dirname(__file__), "../../web/templates"),
            static_path=os.path.join(os.path.dirname(__file__), "../../web/static"),
            cookie_secret="61oETzKTQAGaYdkL5gOmGeJJFuYhNEQnja5dTP1Y/Vo=",
            login_url="/login",
        )
        application = tornado.web.Application(**settings)
        http_server = tornado.httpserver.HTTPServer(application, ssl_options={
            "certfile": os.path.join("./cert/", "certificate.pem"),
            "keyfile": os.path.join("./cert/", "privatekey.pem"),
        })
        http_server.listen(443) 
        tornado.ioloop.IOLoop.instance().start()

    def render_page(self, request_handler, page, template_values = {}):
        tv = {}
        tv["imagesPath"] = self.IMAGES_PATH
        tv["scriptsPath"] = self.SCRIPTS_PATH
        tv["stylesPath"] = self.STYLES_PATH
        tv.update(template_values)
        request_handler.render(page, **tv)
