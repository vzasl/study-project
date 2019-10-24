from flask import Flask, render_template, request, flash, redirect, url_for
import os, subprocess
from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, SubmitField, TextAreaField
from wtforms.validators import DataRequired
from flask_login import LoginManager, UserMixin, login_user, login_required


class RegistrationForm(FlaskForm):
	username = StringField('Username:', id='uname', validators=[DataRequired()])
	password = PasswordField('Password:', id='pword', validators=[DataRequired()])
	two_factor = StringField('2FA: your phone #:', id='2fa', validators=[DataRequired()])
	submit = SubmitField('Register')

class LoginForm(FlaskForm):
	username = StringField('Username:', id='uname', validators=[DataRequired()])
	password = PasswordField('Password:', id='pword', validators=[DataRequired()])
	two_factor = StringField('2FA: your phone #:', id='2fa', validators=[DataRequired()])
	submit = SubmitField('Log in')

class SpellcheckForm(FlaskForm):
	text = TextAreaField('Text to check:', id='inputtext', validators=[DataRequired()])
	submit = SubmitField('Submit')

class Config(object):
	SECRET_KEY = os.environ.get('SECRET_KEY') or '123232480018503588'

class User(UserMixin):
	
	credentials = {'admin' : ['pass', '111']}
	
	def __init__(self, username, password, tfa):
		self.id = username
		self.password = password
		self.tfa = tfa
		self.autthenticated = False
	
	def is_authenticated(self):
		return self.autthenticated
	
	def is_active(self):
		return True
	
	def is_anonymous(self):
		return False
	
	def get_id(self):
		return self.id
	
	@classmethod
	def get(cls, username):
		password = credentials[user_id][0]
		tfa = credentials[user_id][1]
		user_id = username
		return cls(password, tfa, username)

app = Flask(__name__)

app.config.from_object(Config)
login_manager = LoginManager()
login_manager.init_app(app)

@login_manager.user_loader
def load_user(user_id):
	return User.get(user_id)

credentials = {'admin' : ['pass', '111']}
# credentials = {}

@app.route('/')
def home():
	return render_template('home.html')

@app.route('/login', methods=['POST', 'GET'])
def login():
	form = LoginForm()
	if form.validate_on_submit():
		if form.username.data in credentials:
			if form.password.data == credentials[form.username.data][0] and form.two_factor.data == credentials[form.username.data][1]:
				login_user(form.username.data)
				return redirect(url_for('submit'))
		else:
			credentials[form.username.data] = [form.password.data, form.two_factor.data]
			flash('Registration success. Proceed to Log in.')
			return redirect(url_for('login'))
	return render_template('login.html', title='Login', form=form)


@app.route('/register', methods = ['POST', 'GET'])
def register():
	form = RegistrationForm()
	success = ""
	if form.validate_on_submit():
		if form.username.data in credentials:
			success = "failure"
		else:
			success = "success"
			new_user = User(username = form.username.data, password = form.password.data, tfa = form.two_factor.data)
			credentials[form.username.data] = [form.password.data, form.two_factor.data]
			
	return render_template('register.html', title='Register', form=form, success=success)


@app.route('/spell_check', methods=['POST', 'GET'])
@login_required
def submit():
	form = SpellcheckForm()
	text = ""
	miss = ""
	if form.validate_on_submit():
		text = form.text.data
		print(text)
		f = open("text.txt", "w")
		f.write(text)
		f.close()
		cp = subprocess.run("./a.out text.txt wordlist.txt", shell = True, stdout = subprocess.PIPE)
		miss = cp.stdout.decode().replace("\n", ", ")
		if len(miss) > 0:
			miss = miss[:-2]
		print(miss)
		
	return render_template('submit.html', title='Submit text', form=form, text = text, words = miss)

@app.route('/logout')
def logout():
	logout_user()
	return edirect(url_for('home'))

if __name__ == '__main__':
    app.run(debug=True)
