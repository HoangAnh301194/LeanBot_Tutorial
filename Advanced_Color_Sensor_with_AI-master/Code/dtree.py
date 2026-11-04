import pandas as pd
import numpy as np
import glob
import matplotlib.pyplot as plt
from sklearn import tree
from sklearn import metrics
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split
from sklearn.tree import export_text
import time
 
def printDataInfo(df,dataSetLabel):
    print(f"\nDataset {dataSetLabel} Info:\n")
    print(f"Total Number of Samples: {len(df)}")
    print(df['objColor'].value_counts())
 
def CreateTrainTestData(train_pct=0.8):
    # Load all JSON files
    df = pd.concat(
        [pd.read_json(f, orient="records") for f in glob.glob(f"*.json")],
        ignore_index=True
    )
 
    # Remove samples with any negative RGB value
    df = df[~(
        (df['objRed'] < 0) |
        (df['objGreen'] < 0) |
        (df['objBlue'] < 0)
    )]
 
    # Features and labels
    X = df[['objRed', 'objGreen', 'objBlue']]
    y = df['objColor']
 
    # Stratified train/test split
    X_train, X_test, y_train, y_test = train_test_split(
        X, y,
        train_size=train_pct,
        stratify=y,       # <- keep color proportions
        random_state=13,
    )
 
    # Merge X and y back for convenience
    train = X_train.copy()
    train['objColor'] = y_train
 
    test = X_test.copy()
    test['objColor'] = y_test
 
    return train, test
 
def dtree(X_train, y_train):
 
    ## Decision Tree Training
    ## Reference: https://www.w3schools.com/python/python_ml_decision_tree.asp
 
    # Initialize and train the decision tree classifier
    dtree = DecisionTreeClassifier(max_depth=5)
    dtree = dtree.fit(X_train.values, y_train)
    return dtree
 
def confusion_matrix_display(predict_objColor, actual_objColor, labels):
    # Compute a normalized confusion matrix (values between 0 and 1)
    confusion_matrix = metrics.confusion_matrix(y_pred=predict_objColor, y_true=actual_objColor,normalize='true')
    # Create the display object with label names
    cm_display = metrics.ConfusionMatrixDisplay(confusion_matrix = confusion_matrix, display_labels = labels)
    # Print the numeric matrix to the console
    print("Normalized Confusion Matrix:\n", confusion_matrix)       
    # Plot the confusion matrix with better visual formatting
    plt.figure()
    cm_display.plot()
    plt.xticks(rotation=45,ha="right",rotation_mode="anchor") # Rotate x-axis labels for readability
    plt.show()
 
################################################################################################
 
# Train
Train, Test = CreateTrainTestData(train_pct=0.8)
 
# Print info
printDataInfo(Train,'Train')
printDataInfo(Test,'Test')
 
################################################################################################
 
X_train = Train[['objRed', 'objGreen', 'objBlue']]
y_train = Train['objColor']
 
X_test = Test[['objRed', 'objGreen', 'objBlue']]
y_test = Test['objColor']
 
################################################################################################
 
t1 = time.time()*1000
 
# Train the decision tree model using the training set
dtree_model = dtree(X_train, y_train)
 
t2 = time.time()*1000
 
# Use the trained decision tree model to predict the color based on RGB values in the test set
predict_objColor = dtree_model.predict(X_test.values)
 
t3 = time.time()*1000
 
actual_objColor = y_test.to_numpy()
 
# Plotting the decision tree
plt.figure(figsize=(16, 8))  # Set the figure size (in inches)
tree.plot_tree(dtree_model, feature_names=['objRed', 'objGreen', 'objBlue'], fontsize=8)  # Draw the tree with feature names
plt.savefig("dtree.jpg")  # Save the tree diagram as an image
 
rules = export_text(
    dtree_model,
    feature_names=['objRed', 'objGreen', 'objBlue']
)
 
print('\n' + rules + '\n')
 
# Use the class labels ("dtree_model.classes_") from the trained model for display in the confusion matrix
confusion_matrix_display(predict_objColor,actual_objColor,dtree_model.classes_)
 
print(f"Training Elapsed Time: {int(t2 - t1)} ms")
print(f"Testing Elapsed Time: {int(t3 - t2)} ms")
