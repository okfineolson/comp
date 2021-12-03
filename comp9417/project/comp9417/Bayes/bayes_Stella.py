# importing necessary libraries
import numpy as np
import pandas as pd
import seaborn as sn
from sklearn import svm
import matplotlib.pyplot as plt
from sklearn import preprocessing
from sklearn.decomposition import PCA
from sklearn.metrics import confusion_matrix
from sklearn.neural_network import MLPClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.multiclass import OneVsRestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.model_selection import GridSearchCV as grid
from sklearn.gaussian_process import GaussianProcessClassifier
from sklearn.naive_bayes import GaussianNB, BernoulliNB, MultinomialNB
'''
  https://www.analyticsvidhya.com/blog/2020/04/feature-scaling-machine-learning-normalization-standardization/

  Normalization is a scaling technique in which values are shifted and rescaled 
  do that they end up ranging bewteen 0 and 1. (Min-Max Scaling)
  -> Data not follow a Gaussian distribution
  -> useful in algorithms like KNN / NN

  Standardlization is another scaling technique where the value are centered 
  the mean with a unit standard deviation.
  The mean of the attribute becomes zero and the resultant distribution
  has a unit standard deviation.
  -> follows a Guassian distribution
  -> not affected by the outlier
'''

def getData():
  # loading the iris dataset
  X_train = pd.read_csv('MLinTheUnknown/X_train.csv', names=['featxure '+str(i) for i in range(128)]).to_numpy()
  X_test = pd.read_csv('MLinTheUnknown/X_val.csv', names=['feature '+str(i) for i in range(128)]).to_numpy()
  y_train = pd.read_csv('MLinTheUnknown/y_train.csv', names=['target']).to_numpy().reshape(-1)
  y_test = pd.read_csv('MLinTheUnknown/y_val.csv', names=['target']).to_numpy().reshape(-1)
  return X_train, X_test, y_train, y_test

# print('\n––––––––––––––––– Preproccessing –––––––––––––––––')
Multinomial, plot_heat, show_matrix, n_features = False, False, True, 40

def remove_noises(x_train, y_train):
  X_train = x_train
  for feature_index in range(128):
    # X_train = x_train.copy()
    # print(x_train.shape, X_train.shape)
    plt.subplot(1, 2 ,1)
    plt.scatter(np.arange(X_train.shape[0]), X_train[:,feature_index])
    plt.title('Before: Index {}'.format(feature_index))
    values = X_train[:,feature_index]
    max_, min_ = np.max(values), np.min(values)
    # zero list are those values not has a sign of the group 
    sign_list, remove_list = [], []
    # 1. group / noise from one side of median
    # 2. all the values of group is either < 0 or > 0
    if np.mean(values) <= 0:
      threshold = ((max_ + min_)//2 + min_)//2.3
      remove_list = [i for i, val in enumerate(values) if val < threshold ] 
      sign_list = [i for i, val in enumerate(values) if val > 0 ] 
    else:
      threshold = ((max_ + min_)//2 + max_)//2.3
      remove_list = [i for i, val in enumerate(values) if val > threshold ] 
      sign_list = [i for i, val in enumerate(values) if val < 0 ] 
    # check the quantity of the remove list
    # print('Current: {}\n  \tsign_list: {}, length of rm_list: {}'.format(feature_index, sign_list, len(remove_list)))
    if len(remove_list) < 20:
      sign_list += remove_list
    # print('\tsign_list: {}'.format(sign_list))
    # for row in sign_list:
    X_train = np.delete(X_train, sign_list, 0) # del the selected row
    y_train = np.delete(y_train, sign_list) # del the selected row
    plt.subplot(1,2,2)
    plt.title('After: threshold {}'.format(threshold))
    plt.scatter(np.arange(X_train.shape[0]), X_train[:,feature_index])
    plt.savefig('{}.jpg'.format(feature_index))
    plt.clf()
  return X_train, y_train

def plotX_train(name, X_train):
  for i in range(X_train.shape[1]):
    plt.scatter(np.arange(8346), X_train[:,i])
    plt.title('For Feature of index: '+str(i))
    plt.savefig('{}_{}.jpg'.format(name, i))
    plt.clf()

def plotY_train(y_train, name):
  y = np.zeros(6)
  for i in y_train:
    y[int(i-1)] += 1
  y /= y_train.shape[0]
  plt.scatter(np.arange(6), y)
  plt.savefig('distribution'+name+'.jpg')
  
def Standardization(X_train):
  # print('\n ===> Standardization, or mean removal and variance scaling')
  scaler = preprocessing.StandardScaler().fit(X_train)
  mean_pre, std_pre = scaler.mean_, scaler.scale_
  X_train = scaler.transform(X_train)
  mean, std = np.round(X_train.mean(axis=0)), np.round(X_train.std(axis=0))
  # print('X Train RAW: \nMEAN: {}, STD: {}'.format(mean_pre, std_pre))
  # print('X Train Current: \nMEAN: {}, STD: {}\n'.format(mean, std))
  # print('X Train:\n',X_train)
  plotX_train('Standardization', X_train)
  return X_train

def Scaling_range_1(X_train, X_test):
  # print('\n ===> Scaling features to a range')
  # print('\n-> In a range of [0, 1]')
  min_max_scaler = preprocessing.MinMaxScaler()
  X_train = min_max_scaler.fit_transform(X_train)
  X_test = min_max_scaler.fit_transform(X_test)
  # print('X Train:\n',np.round(X_train,3))
  # print('X Test:\n',np.round(X_test,3))
  plotX_train('Scaling1', X_train)
  Multinomial = True
  return X_train, X_test
  
def Scaling_range_2(X_train, X_test):
  # print('\n===> In a range of [-1, 1]')
  max_abs_scaler = preprocessing.MaxAbsScaler()
  X_train = max_abs_scaler.fit_transform(X_train)
  X_test = max_abs_scaler.fit_transform(X_test)
  # print('X Train:\n',np.round(X_train,3))
  # print('X Test:\n',np.round(X_test,3))
  plotX_train('Scaling2', X_train)
  return X_train, X_test

def Normalize(X_train):
  # print("\n ===> Normalize training data")
  X_train = preprocessing.normalize(X_train, norm='l2')
  # print('X Train:\n',np.round(X_train,3))
  plotX_train('normalize', X_train)
  return X_train

def convariance(X_train):
  # print('–––––– covariance martrix ––––––')
  curr = np.reshape(X_train[:,:n_features], (n_features, -1))
  # print('Raw X_train as a shape of:, ', X_train.shape)
  # print('Demenstate X_train as a shape of:, ', X_train[:,:n_features].shape)
  m = np.round(np.cov(curr, bias=True), 2)
  # print('The result of confusion matrix is: \n{}\nShape of confusion matrix is: {}\n'.format(m, m.shape))
  if plot_heat:
    sn.heatmap(m)
    plt.show()

def pca(X_train, X_test):
  pca_model = PCA(n_components=n_features, svd_solver='full')
  pca_model.fit(X_train)
  x_train, x_test = pca_model.fit_transform(X_train), pca_model.fit_transform(X_test)
  # print('AFter PCA: ', x_train.shape)
  convariance(x_train)
  return x_train, x_test
  
X_train, X_test, y_train, y_test = getData()
X_train, y_train = remove_noises(X_train, y_train)
print('After removing the noises, X_train is a shape of : ', X_train)
# plotX_train('initial', X_train)
# X_train, X_test = Standardization(X_train), Standardization(X_test)
# X_train, X_test = Scaling_range_1(X_train, X_test)
# X_train, X_test = Scaling_range_2(X_train, X_test)
# X_train, X_test = Normalize(X_train), Normalize(X_test)
# convariance(X_train)
# plotY_train(y_test, 'y_test')
# plotY_train(y_train, 'y_train')
  
# print('\n––––––––––––––––– Start Training –––––––––––––––––')

'''
USING RAW DATA:
  –––––– Standardization –––––––
    –––––– Guassian –––––––
    Accuracy:
    0.22429906542056074
    Confusion Matrix:
    [[  2   0   0   0 510   0]
    [  0   0   0   0 552   0]
    [  0   0   0   0 327   0]
    [  0   0   0   0 408   0]
    [  0   0   0   0 622   0]
    [  0   0   0   0 361   0]]

    –––––– Bernoulli –––––––
    Accuracy:
    0.18404025880661395
    Confusion Matrix:
    [[512   0   0   0   0   0]
    [552   0   0   0   0   0]
    [327   0   0   0   0   0]
    [408   0   0   0   0   0]
    [622   0   0   0   0   0]
    [361   0   0   0   0   0]]
  
  –––––– Scaling_range_1 –––––––
    –––––– Guassian –––––––
    Accuracy:
    0.18296189791516895
    Confusion Matrix:
    [[509   0   3   0   0   0]
    [552   0   0   0   0   0]
    [327   0   0   0   0   0]
    [408   0   0   0   0   0]
    [622   0   0   0   0   0]
    [361   0   0   0   0   0]]

    –––––– Bernoulli –––––––
    Accuracy:
    0.22537742631200575
    Confusion Matrix:
    [[  4   2   0   0 506   0]
    [  3   3   0   0 546   0]
    [  0   1   2   0 324   0]
    [  2   0   1   0 405   0]
    [  2   1   2   0 617   0]
    [  0   3   0   0 357   1]]

    –––––– MultinomialNB ––––––
    Accuracy:
    0.5510424155283968
    Confusion Matrix:
    [[389   1   0  42   1  79]
    [ 63 365  19  15   1  89]
    [ 62   0 116  19   0 130]
    [125   0   0  68   4 211]
    [114   0   0  34 281 193]
    [ 23   0   0  16   8 314]]
      
  –––––– Scaling_range_2 –––––––
    –––––– Guassian –––––––
    Accuracy:
    0.18404025880661395
    Confusion Matrix:
    [[512   0   0   0   0   0]
    [552   0   0   0   0   0]
    [327   0   0   0   0   0]
    [408   0   0   0   0   0]
    [622   0   0   0   0   0]
    [361   0   0   0   0   0]]

    –––––– Bernoulli –––––––
    Accuracy:
    0.21099928109273905
    Confusion Matrix:
    [[  1 506   0   0   0   5]
    [  1 547   0   0   0   4]
    [  0 312  15   0   0   0]
    [  1 403   0   4   0   0]
    [  0 621   0   0   0   1]
    [  1 340   0   0   0  20]]

  –––––– Normalize ––––––
    –––––– Guassian –––––––
    Accuracy:
    0.1445003594536305
    Confusion Matrix:
    [[379 133   0   0   0   0]
    [532  11   9   0   0   0]
    [301   6  11   2   0   7]
    [287 121   0   0   0   0]
    [ 14 607   0   0   1   0]
    [108 252   0   1   0   0]]

    –––––– Bernoulli –––––––
    Accuracy:
    0.21099928109273905
    Confusion Matrix:
    [[  1 506   0   0   0   5]
    [  1 547   0   0   0   4]
    [  0 312  15   0   0   0]
    [  1 403   0   4   0   0]
    [  0 621   0   0   0   1]
    [  1 340   0   0   0  20]]

AFTER PCA:
  ––––––––––– Number of Features: 10 –––––––––––
  Guassian : 0.6107117181883537
  Bernoulli : 0.49784327821711

  ––––––––––– Number of Features: 20 –––––––––––
  Guassian : 0.5093457943925234
  Bernoulli : 0.41337167505391803

  ––––––––––– Number of Features: 30 –––––––––––
  Guassian : 0.4798705966930266
  Bernoulli : 0.41876347951114307

  ––––––––––– Number of Features: 40 –––––––––––
  Guassian : 0.45830337886412653
  Bernoulli : 0.3964773544212797

  ––––––––––– Number of Features: 50 –––––––––––
  Guassian : 0.4539899352983465
  Bernoulli : 0.40007189072609634

  ––––––––––– Number of Features: 60 –––––––––––
  Guassian : 0.45578720345075485
  Bernoulli : 0.41013659237958305

  ––––––––––– Number of Features: 70 –––––––––––
  Guassian : 0.4543493889288282
  Bernoulli : 0.40869877785765635

  ––––––––––– Number of Features: 80 –––––––––––
  Guassian : 0.4547088425593098
  Bernoulli : 0.40797987059669305

  ––––––––––– Number of Features: 90 –––––––––––
  Guassian : 0.4547088425593098
  Bernoulli : 0.3989935298346513

  ––––––––––– Number of Features: 100 –––––––––––
  Guassian : 0.4550682961897915
  Bernoulli : 0.3946800862688713

  ––––––––––– Number of Features: 110 –––––––––––
  Guassian : 0.4550682961897915
  Bernoulli : 0.3856937455068296

  ––––––––––– Number of Features: 120 –––––––––––
  Guassian : 0.4550682961897915
  Bernoulli : 0.38497483824586626   

RESCALE(-1,1) + N_Feature = 14:
  –––––– Guassian –––––––         –––––– Bernoulli –––––––
  Accuracy : 
  0.5607476635514018              0.37814521926671457
  Confusion Matrix:
  [[115 195   1  64 122  15]      [[ 89 316   0  36  33  38]
  [ 35 338  68  88  13  10]       [191 158  85  80  36   2]
  [  8   0 227  79  13   0]       [ 17  67 152  75  12   4]
  [ 16  35  58 197  26  76]       [ 52  48  62  85  99  62]
  [ 19   0   3  51 425 124]       [ 61   0  32  52 401  76]
  [ 29   4   3  41  26 258]]      [ 30   2   0  51 111 167]]
'''

def bayes_getParamater(X_train, X_test, y_train, y_test):
  print('\n––––––––––– Number of Features: {} –––––––––––'.format(n_features))
  # print('\n––––––––––––––––– Bayes –––––––––––––––––')
  print('–––––– Guassian –––––––')
  gnb = GaussianNB().fit(X_train, y_train)
  gnb_predictions = gnb.predict(X_test)
  # accuracy on X_test
  accuracy = gnb.score(X_test, y_test)
  print('Accuracy :',accuracy)
  if show_matrix:
    # creating a confusion matrix
    cm = confusion_matrix(y_test, gnb_predictions)
    print('Confusion Matrix:\n',cm)
  
  print('–––––– Bernoulli –––––––')
  bnb = BernoulliNB().fit(X_train, y_train)
  bnb_predictions = bnb.predict(X_test)
  # accuracy on X_test
  accuracy = bnb.score(X_test, y_test)
  print('Accuracy :',accuracy)
  if show_matrix:
    # creating a confusion matrix
    cm = confusion_matrix(y_test, bnb_predictions)
    print('Confusion Matrix:\n',cm)
  
  if Multinomial:
    print('\n–––––– MultinomialNB ––––––')
    mnb = MultinomialNB().fit(X_train, y_train)
    mnb_predictions = mnb.predict(X_test)
    # accuracy on X_test
    accuracy = mnb.score(X_test, y_test)
    print('Accuracy:',accuracy)
    if show_matrix:
      # creating a confusion matrix
      cm = confusion_matrix(y_test, mnb_predictions)
      print('Confusion Matrix:\n',cm)

def bayes():
  for n in range(10, 20, 1):
    n_features = n
    x_train, x_test = pca(X_train, X_test)
    bayes_getParamater(x_train, x_test, y_train, y_test)
bayes()

MLP_VARIABLES = {
  'activation' :('logistic', 'tanh', 'relu'),
  # 'alpha': np.arange(0.01, 0.1, 0.005),
  # 'l_init': np.arange(0.01, 0.1, 0.01),
  # 'momentum': np.arange(0, 1.2, 0.1),
  # 'h_sizes': layers,
}
# layers = [(100, 50), (90, 45), (80, 40), (70, 35), (60, 30)]
# act_funcs = ['logistic', 'tanh', 'relu']
def mlp_getParamater():
  print('––––––– MLP Classifier –––––––')
  for act in act_funcs:
    for layer in layers:
      mlp = MLPClassifier(solver='adam', 
                          random_state = 1, 
                          activation = 'tanh',
                          hidden_layer_sizes = layer,
                        ).fit(X_train, y_train)
  accuracy = mlp.score(X_test, y_test)
  print('Accuracy: ', accuracy)
  mlp_predictions = mlp.predict(X_test) 
  cm = confusion_matrix(y_test, mlp_predictions)
  print('Confusion Matrix:\n',cm)

print('The shape of X_train: ', X_train.shape)
# mlp_getParamater()

'''
  VARIABLES = { 
    'knn': {
      'n_neighbors': np.arange(1, 10),
      'algorithm': ('auto', 'ball_tree', 'kd_tree', 'brute'),
      'leaf_size': np.arange(2, 50, 2),
      'p': np.arange(1, 5)
    },
    'bayes': {
      # 'var_smoothing': np.arange(0, 2 ,0.1)
    },  
    'linearSVC': {
      'loss': ('hinge', 'squared_hinge'),
    },
    'logistic': {
      'penalty': ('l1', 'l2', 'elasticnet'),
    }
  }

  ––––––– OVR Logistic –––––––
  Best Paramaters:  {'penalty': 'l2'}
  Accuracy:
  0.6214953271028038
  Confusion Matrix:
  [[476   0   0   0   0  36]
  [ 13 455   0   2   1  81]
  [  5   1 117   0   1 203]
  [ 36   0   0  66   0 306]
  [ 11   0   0   8 254 349]
  [  0   0   0   0   0 361]]

  ––––––– OVR Linear –––––––
  Best Paramaters:  {'loss': 'squared_hinge', 'penalty': 'l2'}
  Accuracy:
  0.612508986340762
  Confusion Matrix:
  [[461   1   0   0   0  50]
  [  8 473   0   1   2  68]
  [ 24   2  70   0   1 230]
  [ 67   0   0  30   0 311]
  [  2   0   0   0 309 311]
  [  0   0   0   0   0 361]]

  # Best Paramaters:  {'algorithm': 'auto', 'n_neighbors': 1, 'p': 1}
  # Accuracy:
  # 0.9295470884255931
  # Confusion Matrix:
  # [[500   2   0  10   0   0]
  # [  3 532   0   1   1  15]
  # [  0   8 284   2   3  30]
  # [  0   0   0 368   0  40]
  # [  0   0   3   9 557  53]
  # [  0   0   0  15   1 345]]

def knn_getParamater():
  print('\n––––––––––––––––– KNN –––––––––––––––––')
  # training a KNN classifier
  knn = KNeighborsClassifier()
  # clf = grid(knn, VARIABLES['knn']).fit(X_train, y_train)
  # print('Best Paramaters: ', clf.best_params_)
  # accuracy on X_test
  accuracy = knn.score(X_test, y_test)
  # print accuracy
  print('Accuracy:\n',accuracy)
  # creating a confusion matrix
  knn_predictions = knn.predict(X_test) 
  cm = confusion_matrix(y_test, knn_predictions)
  print('Confusion Matrix:\n',cm)

def knn(n, algo, leaf_size, p):
  print('\n––––––––––––––––– KNN –––––––––––––––––')
  knn = KNeighborsClassifier()
  # accuracy on X_test
  accuracy = knn.score(X_test, y_test)
  # print accuracy
  print('Accuracy:\n',accuracy)
  # creating a confusion matrix
  knn_predictions = knn.predict(X_test) 
  cm = confusion_matrix(y_test, knn_predictions)
  print('Confusion Matrix:\n',cm)

def ovr(Logistic=True):
  # define model
  if Logistic:
    print('\n––––––– OVR Logistic –––––––')
    ovr = grid(LogisticRegression(multi_class='ovr'), VARIABLES['logistic'])
  else:
    print('\n––––––– OVR Linear –––––––')
    ovr = grid(svm.LinearSVC(multi_class='ovr'), VARIABLES['linearSVC'])
  
  # define the ovr strategy
  # fit model
  ovr.fit(X_train, y_train)
  # make predictions
  ovr_predictions = ovr.predict(X_test)
  
  # accuracy on X_test
  print('Best Paramaters: ', ovr.best_params_)
  accuracy = ovr.score(X_test, y_test)
  print('Accuracy:\n',accuracy)

  # print(ovr_predictions)
  # creating a confusion matrix
  cm = confusion_matrix(y_test, ovr_predictions)
  print('Confusion Matrix:\n',cm)
  # print('\n––––––––––––––––– OVR –––––––––––––––––')
  # ovr()
  # ovr(False)

  # Olson, Boammels
'''
