import pandas as pd
import datetime
import numpy
from sklearn.metrics import accuracy_score
from sklearn.model_selection import GridSearchCV
from sklearn.naive_bayes import GaussianNB 
from sklearn.naive_bayes import BernoulliNB 
from sklearn.naive_bayes import MultinomialNB 
from sklearn.metrics import confusion_matrix
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import StandardScaler
X_test = pd.read_csv('X_test.csv')
X_train = pd.read_csv('X_train.csv')
X_val = pd.read_csv('X_val.csv')
y_train = pd.read_csv('y_train.csv')
y_val = pd.read_csv('y_val.csv')
#bayes
def bayes():
  gnb = GaussianNB() #构造
  pred = gnb.fit(X_train,y_train) #拟合
  y_pred = pred.predict(X_val) #预测
  print("GaussianNB accuracy:",pred.score(X_val, y_val))
  '''
  cm = confusion_matrix(y_val, y_pred)
  print('Confusion Matrix:\n',cm)
   [[235   1  11 115  49 101]
 [ 14 351  73  17  10  87]
 [  1  13 169  11   6 127]
 [ 21   0   0 131  10 246]
 [  0   0   1  97 342 181]
 [  0   0   0  31  14 316]]
  '''
  gnb = BernoulliNB() #构造
  pred = gnb.fit(X_train,y_train) #拟合
  y_pred = pred.predict(X_val) #预测
  print("BernoulliNB accuracy:",pred.score(X_val, y_val))
  #GaussianNB accuracy: 0.5551959726717008
  #BernoulliNB accuracy: 0.2373247033441208
bayes()
