from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    #set total value of portfolio to 0
    totalPortfolioValue = 0
    
    #pull data from portfolio table
    companies = db.execute("SELECT * FROM portfolio WHERE id = :id", id = session["user_id"])
    
    #iterate over the different companies and its data from user's portfolio
    for company in companies:
        ticker = company["ticker"]
        stockInfo = lookup(ticker)
        stockPresentValue = company["amount"] * stockInfo["price"]
        db.execute("UPDATE portfolio SET curprice = :curprice, total = :total WHERE ticker = :ticker", ticker = ticker, curprice = usd(float(stockInfo["price"])), total = usd(stockPresentValue))
        #add total value of each holding to total portfolio value
        totalPortfolioValue += stockPresentValue
    #pull cash data from user table
    user = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    #add in how much cash user currently has
    totalPortfolioValue += user[0]["cash"]
    
    #display information into html
    return render_template("index.html", companies = companies, cash = usd(user[0]["cash"]), TPV = usd(totalPortfolioValue))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        #check to see if input fields are empty
        if not request.form.get("ticker"):
            return apology("provide a stock symbol")
        elif not request.form.get("amount"):
            return apology("amount cannot be empty")
        elif 0 > int(request.form.get("amount")):
            return apology("amount cannot be negative")
        #call lookup functon for info on company
        stock = lookup(request.form.get("ticker").upper())
        #check to see if user entered a valid stock ticker
        if not stock:
            return apology("stock ticker cannot be found")
        #pull data from user table
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        #calculate the total value of shares the user wants to purchase
        stockTotal = stock["price"] * float(request.form.get("amount"))
        #check if user has enough cash to purchase
        if cash[0]["cash"] > stockTotal:
            #update user cash balance
            db.execute("UPDATE users SET cash = :balance WHERE id = :id", balance = (cash[0]["cash"] - stockTotal), id = session["user_id"])
            #log transaction in history table
            db.execute("INSERT INTO history ('id', 'type', 'ticker', 'amount', 'price', 'total') VALUES (:id, :type, :ticker, :amount, :price, :total)", id = session["user_id"], type = "buy", ticker = stock["symbol"], amount = request.form.get("amount"), price = usd(stock["price"]), total = usd(stockTotal))
           
            #update SQL portfolio
            portfolioShares = db.execute("SELECT amount FROM portfolio WHERE id = :id AND ticker = :ticker", id = session["user_id"], ticker = stock["symbol"])
            #if stock does not exist in portfolio then create new
            if not portfolioShares:
                db.execute("INSERT INTO portfolio ('id', 'ticker', 'amount', 'curprice', 'total') VALUES (:id, :ticker, :amount, :curprice, :total)", id = session["user_id"], ticker = stock["symbol"], amount = request.form.get("amount"), curprice = usd(stock["price"]), total = usd(stockTotal))
                return redirect(url_for("index"))
            portfolioAmount = portfolioShares[0]["amount"] + float(request.form.get("amount"))
            portfolioUpdate = db.execute("UPDATE portfolio SET amount = :portfolioAmount, curprice = :curprice, total = :portfolioTotal WHERE id = :id AND ticker = :ticker", portfolioAmount = portfolioAmount, portfolioTotal = usd(portfolioAmount * stock["price"]), curprice = usd(stock["price"]), id = session["user_id"], ticker = stock["symbol"])
            return redirect(url_for("index"))
        return apology("not enough cash")
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    history = db.execute("SELECT * FROM history WHERE id = :id", id = session["user_id"])
    if not history:
        return apology("no history of transactions found")
    return render_template("history.html", history = history)
    
@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit cash into your account"""
    if request.method == "POST":
        if not request.form.get("amount"):
            return apology("you did not provide an amount for deposit")
        if float(request.form.get("amount")) > 10000.00:
            return apology("you can only deposit up to $10,000 per transaction")
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        db.execute("UPDATE users SET cash = :balance WHERE id = :id", id = session["user_id"], balance = float(request.form.get("amount")) + cash[0]["cash"])
        db.execute("INSERT INTO history ('id', 'type', 'ticker', 'amount', 'price', 'total') VALUES (:id, :type, :ticker, :amount, :price, :total)", id = session["user_id"], type = "deposit", ticker = " ", amount = " ", price = " ", total = usd(float(request.form.get("amount"))))
        return redirect(url_for("index"))
    else:
        return render_template("deposit.html")
        
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        #check if stock ticker is valid or is not empty
        if not request.form.get("quote"):
            return apology("enter a valid stock ticker")
        #call lookup function in helpers.py to get info on quote
        info = lookup(request.form.get("quote"))
        #check to see if stock ticker exists
        if not info:
            return apology("stock ticker cannot be found")
        #returns info from lookup and displays it on a separate html
        return render_template("quoted.html", company = info["name"], symbol = info["symbol"], price = info["price"])
    else:
        return render_template("quote.html")
        

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    #ensures username and password fields are not blank
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must create a username")
        elif not request.form.get("password"):
            return apology("must create a password")
        elif not request.form.get("passwordre"):
            return apology("must re-enter password")
                    
        #check to see if passwords match
        if request.form.get("password") != request.form.get("passwordre"):
            return apology("passwords do not match")
                
        #create username and password
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hashed)", username=request.form.get("username"), hashed=pwd_context.hash(request.form.get("password")))
        
        if not result:
            return apology("username already exists")

        session["user_id"] = result
        return redirect(url_for("index"))
    
    else:
        return render_template("register.html")
    
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        #check to see if input fields are empty
        if not request.form.get("ticker"):
            return apology("provide a stock symbol")
        elif not request.form.get("amount"):
            return apology("amount cannot be empty")
            
        numShares = db.execute("SELECT amount FROM portfolio WHERE id = :id AND ticker = :ticker", id = session["user_id"], ticker = request.form.get("ticker").upper())
        if not numShares:
            return apology("this stock is not apart of your portfolio")
        if float(request.form.get("amount")) > numShares[0]["amount"]:
            return apology("you cannot sell more than you own")
        #call lookup function for info on company
        stock = lookup(request.form.get("ticker"))
        #check to see if user entered a valid stock ticker
        if not stock:
            return apology("stock ticker cannot be found")
        #calculate the total value of shares the user wants to sell
        stockTotal = stock["price"] * float(request.form.get("amount"))
        db.execute("INSERT INTO history ('id', 'type', 'ticker', 'amount', 'price', 'total') VALUES (:id, :type, :ticker, :amount, :price, :total)", id = session["user_id"], type = "sell", ticker = stock["symbol"], amount = request.form.get("amount"), price = usd(stock["price"]), total = usd(stockTotal))

        #pull data from user table
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        #update user cash balance
        db.execute("UPDATE users SET cash = :balance WHERE id = :id", balance = (cash[0]["cash"] + stockTotal), id = session["user_id"])

        #update SQL portfolio
        portfolioShares = db.execute("SELECT amount FROM portfolio WHERE id = :id AND ticker = :ticker", id = session["user_id"], ticker = stock["symbol"])
        portfolioAmount = portfolioShares[0]["amount"] - float(request.form.get("amount"))
        portfolioUpdate = db.execute("UPDATE portfolio SET amount = :portfolioAmount, total = :portfolioTotal WHERE id = :id AND ticker = :ticker", portfolioAmount = portfolioAmount, portfolioTotal = usd(portfolioAmount * stock["price"]), id = session["user_id"], ticker = stock["symbol"])
        return redirect(url_for("index"))
            
    else:
        return render_template("sell.html")
