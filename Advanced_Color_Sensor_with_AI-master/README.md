# Object Color Classification with AI Model

This guide explains how to build, train, and deploy a Decision Tree model on an Arduino Leanbot for real-time object color classification based on RGB sensor readings.

## Table of Contents

- [1. Data Collection and Preparation with Leanbot](#1.-data-collection-and-preparation-with-leanbot)
  + [1.1. Leanbot Data Collection Tool](#1.1.-leanbot-data-collection-tool)
  + [1.2. Data Collection Experiments](#1.2.-data-collection-experiments)
  + [1.3. Data Preparation](#1.3.-data-preparation)
  + [1.4. Example Data](#1.4.-example-data)
- [2. Model Creation with Python](#2.-model-creation-with-python)
  + [2.1. Data Splitting (Training and Testing)](#2.1.-data-splitting-%28training-and-testing%29)
  + [2.2. Model Training](#2.2.-model-training)
  + [2.3. Model Evaluation / Testing](#2.3.-model-evaluation-%2f-testing)
  + [2.4. Example Model](#2.4.-example-model)
- [3. Model Deployment on Leanbot](#3.-model-deployment-on-leanbot)
  + [3.1. Convert Model into Leanbot Code with Generative AI](#3.1.-convert-model-into-leanbot-code-with-generative-ai)
  + [3.2. Integration into Leanbot program](#3.2.-integration-into-leanbot-program)
  + [3.3. Running on Leanbot](#3.3.-running-on-leanbot)

---

## 1. Data Collection and Preparation with Leanbot

### 1.1. Leanbot Data Collection Tool

The following source code is used on the Leanbot to measure the RGB reflection of an object and output the raw data in JSON format via the Serial port.
- Example Data Collection code: [samplingColor.ino](https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/blob/master/Code/samplingColor.ino)

#### Customization: Label and Color Settings

To define the colors sampled, modify the following key variables in the code:
- `objColorNum`: Total number of colors to measure.
- `objColor[]`: Names of the colors, defining the order in which the Leanbot collects data.
- `objColorRGB[]`: RGB values for each color that the Leanbot will display on its LED.
- `objColorIndex`: Keeps track of the current color being measured.

```C 
#define objColorNum    5
const char* objColor[objColorNum] = {"RED", "YELLOW", "GREEN", "BLUE", "PURPLE"}; 
const uint32_t objColorNum[objColorNum] = {0xFF0000, 0xFFFF00, 0x00FF00,0x0000FF, 0xFF00FF};
int objColorIndex = 0;
```

How to modify the colors:
- To use different colors or a different order, change the `objColor[]` array.
- Update the `objColorRGB[]` array with the corresponding RGB values.
- Adjust `objColorNum` to match the total number of colors you want to collect.
- This way, you can customize the set of colors for your data collection without changing the main logic of the code.

#### How to use

**Steps**:
<ol>
  <li>Power on the Leanbot and connect it to your computer via USB.</li>
  <li>Open the Serial Monitor.</li>
  <li>At start, <code>objColor</code> begins with YELLOW. Place a yellow object (e.g., a yellow Jenga block) in front of the Leanbot, between the grippers, within 5 cm.</li>
  <li>Press the TB2A button: the Leanbot flashes its RGB LED and measures the object's color.
    <ul>
      <li>For each measurement, place the object with slight variations (distance, rotation angle, small offsets between the grippers) to introduce natural randomness.</li>
    </ul>
  </li>
  <li>The Leanbot prints the following JSON to the Serial Monitor:
    <pre>{
  "objColor": "YELLOW",
  "objRed": 17,
  "objGreen": 20,
  "objBlue": 4
}</pre>
  </li>
  <li>Press the button multiple times (e.g., 20 times): the buzzer beeps, and the next color is selected (GREEN).</li>
  <li>Place an object matching the new color, repeat the measurements, and continue through BLUE, PURPLE, and RED.</li>
  <li>After RED, the cycle repeats starting from YELLOW. One full sampling cycle is complete, and the data can be saved to a JSON file.</li>
</ol>

#### Save data to Json file 

**Save data to Json file **
1. Copy all content from the Serial Monitor into a text file.  
2. Add `[` at the beginning of the file and `]` at the end.  
3. Save the file in JSON format (`.json`).  

> Example: original data copied from the Serial Monitor

```
AT+NAME
LB639199                            <- Replace this line with "["
{
"objColor":"YELLOW",
"objRed":17,
"objGreen":20,
"objBlue":4,
},
{
"objColor":"YELLOW",
"objRed":9,
"objGreen":11,
"objBlue":2,
},

...

{
"objColor":"RED",
"objRed":6,
"objGreen":0,
"objBlue":1,
},
{
"objColor":"RED",
"objRed":14,
"objGreen":1,
"objBlue":1,
},                                <- Replace "," with "]"     
```

> After modification (valid JSON file): [Example JSON File](https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/blob/master/Data/Indoor%20with%20Normal%20Lighting%203.json)

```
[
{
"objColor":"YELLOW",
"objRed":17,
"objGreen":20,
"objBlue":4,
},
{
"objColor":"YELLOW",
"objRed":9,
"objGreen":11,
"objBlue":2,
},

...

{
"objColor":"RED",
"objRed":6,
"objGreen":0,
"objBlue":1,
},
{
"objColor":"RED",
"objRed":14,
"objGreen":1,
"objBlue":1,
}]    
```

### 1.2. Data Collection Experiments

#### Different condition

- **Location**: Indoor, outdoor.
- **Lighting conditions**: Normal, obscured light, Strong light, Direct light.

#### Data Collection Tips

When collecting samples, try to cover diverse, varying real-world conditions::
- Vary object position, rotation, and slight offsets to introduce natural randomness.
- Keep the object within a short distance (e.g., within 5-8 cm).  
- Object surfaces: measure different faces of the wooden block.  
- Object orientation: rotate the object to different angles.  
- Position variation: change distance and allow slight offsets from the center axis.
- Multiple objects of the same color

#### Example Object Placement Images

> Experimental setup for RGB data collection using Leanbot:

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/da.jpg" width="50%" />
</p>

> Red object placed in front of the Leanbot when the RGB LED is **RED**.

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/red.jpg" width="50%" />
</p>

> Green object placed in front of the Leanbot when the RGB LED is **GREEN**.

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/green.jpg" width="50%" />
</p>

> Blue object placed in front of the Leanbot when the RGB LED is **BLUE**.

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/blue.jpg" width="50%" />
</p>


> Yellow object placed in front of the Leanbot when the RGB LED is **YELLOW**.

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/yellow.jpg" width="50%" />
</p>


> Purple object placed in front of the Leanbot when the RGB LED is **PURPLE**.

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/purple.jpg" width="50%" />
</p>

#### 1.3. Data Preparation

- Repeat the data collection procedure described above multiple times (one collection = one full sampling cycle of the code).  
- Each collection produces one JSON file for training.  
- Collecting more samples helps the Decision Tree handle noise and distinguish similar colors.  

#### 1.4. Example Data

In this guide, three sample files are provided as examples (students are encouraged to collect more for better results):  

| JSON File                                                                                                                                                                                                               | Number of Samples | Number of Colors | Location | Lighting Condition | Collection Time (minutes) |
| :---- | :----: | :----: | :----: | :----: | :----: |
| [Indoor with Normal Lighting 1.json](https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/blob/master/Data/Indoor%20with%20Normal%20Lighting%201.json) |        100        |         5        |  Indoors |       Normal       |             3             |
| [Indoor with Normal Lighting 2.json](https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/blob/master/Data/Indoor%20with%20Normal%20Lighting%202.json) |        100        |         5        |  Indoors |       Normal       |             3             |
| [Indoor with Normal Lighting 3.json](https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/blob/master/Data/Indoor%20with%20Normal%20Lighting%203.json) |        100        |         5        |  Indoors |       Normal       |             3             |

## 2. Model Creation with Python

### 2.1. Data Splitting (Training and Testing)

This section explains how to prepare the training and testing datasets from your sample JSON files.
- Make sure all your collected JSON files are uploaded to the PNote workspace before running the code.
- Basics guide: https://www.w3schools.com/python/python_ml_train_test.asp
- `train_test_split` Reference: https://scikit-learn.org/stable/modules/generated/sklearn.model_selection.train_test_split.html

#### Preparing Training and Testing Sets

The following function loads all JSON files, cleans invalid samples, and splits the data into training and testing sets:

```python
def CreateTrainTestData(train_pct=0.8):
    # Load all JSON files
    df = pd.concat(
        [pd.read_json(f, orient="records") for f in glob.glob(f"*.json")],
        ignore_index=True
    )

    # Remove samples with any negative RGB value (invalid samples)
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
		random_state=13,  # <- for reproducible
    )

    # Merge X and y back for convenience
    train = X_train.copy()
    train['objColor'] = y_train

    test = X_test.copy()
    test['objColor'] = y_test

    return train, test
```

**What it does:** 
- Reads all `.json` files in the current PNote directory (e.g., the three sample files uploaded earlier).  
- Merges them into a single dataset.  
- Removes invalid samples with negative RGB values.  
- Uses RGB values as features and `objColor` as the label.  
- Splits the data into training and testing sets while keeping color proportions balanced.
- Note on random_state:
  + random_state is used only to make the train/test split reproducible.
  + With the same random_state, you will always get the same split, which is useful for debugging and documentation.
  + You can remove this parameter or set it to None to achieve a truly random split each time.

**Usage:**
- `train_pct`: fraction of data used for training (default: 80%).
- `train`: training dataset.
- `test`: testing dataset.

This prepares clean and balanced data for training and evaluating the Decision Tree.

```python
train, test = CreateTrainTestData(train_pct=0.8)
```

#### Inspecting the Dataset

The function below prints basic information about a dataset, including the total number of samples and the distribution of colors.

```Python
def printDataInfo(df,dataSetLabel):
    print(f"\nDataset {dataSetLabel} Info:\n")
    print(f"Total Number of Samples: {len(df)}")
    print(df['objColor'].value_counts())
```

**What it shows:**  
- Total number of collected samples.  
- Number of samples for each color class.

**Usage:**

```python
printDataInfo(train, "Train")
printDataInfo(test, "Test")
```

**Result:**

```
Dataset Train Info:

Total Number of Samples: 239
objColor
BLUE      48
GREEN     48
RED       48
PURPLE    48
YELLOW    47
Name: count, dtype: int64

Dataset Test Info:

Total Number of Samples: 60
objColor
BLUE      12
PURPLE    12
GREEN     12
YELLOW    12
RED       12
Name: count, dtype: int64
```

Interpretation:
- The training and testing datasets are created successfully.
- Color classes are evenly distributed in both sets.
- This confirms that the data split is balanced and suitable for training and evaluation.

### 2.2. Model Training

This section covers how to train a Decision Tree model using the prepared training dataset.
The workflow consists of:
- Training a Decision Tree using the prepared training dataset
- Predicting color labels for the test dataset
- Evaluating the model using a normalized confusion matrix
- Visualizing the learned decision rules
- Reference: https://www.w3schools.com/python/python_ml_decision_tree.asp

A complete example showing how to train, test, and evaluate the Decision Tree using the dataset built in the previous section is provided here:
- **Example code:** [dtree.py](https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/blob/master/Code/dtree.py)

#### Define Features and Labels

The Decision Tree model is trained using RGB values (`objRed`, `objGreen`, `objBlue`) as input features and `objColor` as the target label.

Here, the training and testing datasets created earlier are separated into features (X) and labels (y):
- `X_train, X_test`: RGB values used as model inputs
- `y_train, y_test`: ground-truth color labels

```Python 
X_train = Train[['objRed', 'objGreen', 'objBlue']]
y_train = Train['objColor']
 
X_test = Test[['objRed', 'objGreen', 'objBlue']]
y_test = Test['objColor']
```

#### Training the Decision Tree

The function below creates and trains a Decision Tree classifier:
- `DecisionTreeClassifier(...)`: creates a Decision Tree model from scikit-learn
- max_depth=5: limits how many levels the tree can grow
  + Keeps the model small and easy to understand
  + Avoids overly complex trees caused by noise
- `.fit(X_train, y_train)`: trains the model using:
  + X_train: RGB feature values
  + y_train: corresponding color labels

```Python
def dtree(X_train, y_train):
 
    ## Decision Tree Training
    ## Reference: https://www.w3schools.com/python/python_ml_decision_tree.asp
 
    # Initialize and train the decision tree classifier
    dtree = DecisionTreeClassifier(max_depth=5)
    dtree = dtree.fit(X_train.values, y_train)
    return dtree
```

Once the function is defined, the model is trained with a single line:

```python
dtree_model = dtree(X_train, y_train)
```

### 2.3. Model Evaluation / Testing

#### Test the Trained Model

After training, the Decision Tree model is tested using the test dataset:
- `X_test` contains RGB values that were not used during training.
- `.predict(...)` applies the trained Decision Tree to these RGB inputs.
- `predict_objColor` stores the predicted color labels for each test sample.

```python 
# Use the trained decision tree model to predict the color based on RGB values in the test set
predict_objColor = dtree_model.predict(X_test.values)
```

These predictions are later compared with the true labels (y_test) using a confusion matrix to evaluate model performance.

#### Model evaluation: Confusion Matrix

The confusion matrix shows how well the model predicts each color class.
- It is an **N × N matrix**, where:
  - **Columns** represent the true labels.
  - **Rows** represent the predicted labels.
- Each cell *(i, j)* indicates how many samples of class *j* were predicted as class *i*.
- In this guide, the matrix is **row-normalized**, meaning:
  - The values in each row sum to 1.
  - Each value represents the prediction ratio for a given true class.

Use the function below to compute and visualize the confusion matrix:

```
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
```

**Usage:**

```Python 
# Use the class labels ("dtree_model.classes_") from the trained model for display in the confusion matrix
confusion_matrix_display(predict_objColor,actual_objColor,dtree_model.classes_)
```

> Example: Confusion Matrix plot for the test dataset

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/DocsConfusionMatrix.jpg" />
</p>

> Example: Text-based Confusion Matrix of the test dataset:

```
Normalized Confusion Matrix:
 [[0.91666667 0.         0.08333333 0.         0.        ]
 [0.         0.91666667 0.         0.         0.08333333]
 [0.         0.         1.         0.         0.        ]
 [0.         0.         0.         1.         0.        ]
 [0.         0.         0.         0.         1.        ]]
```

Interpretation from the matrix:
- **BLUE objects**:  
  + ~92% of samples are correctly predicted as **BLUE**.  
  + ~8.3% are misclassified as **PURPLE**.
- **GREEN objects**:  
  + ~92% of samples are correctly predicted as **GREEN**.  
  + ~8.3% are misclassified as **YELLOW**.
- **PURPLE, RED, and YELLOW objects**:  
  + 100% of samples are correctly classified with no mispredictions.

### 2.4. Example Model

To understand how our trained Decision Tree makes decisions, we can visualize it both graphically and as text-based rules. 
- These examples are directly from the model trained on our collected RGB dataset.

#### Graphical visualization

The following code generates an image illustrating:
- RGB feature splits at each node of the Decision Tree  
- The decision paths used by the model to classify different colors  

```Python 
# Plotting the decision tree
plt.figure(figsize=(50, 30))  # Set the figure size (in inches)
tree.plot_tree(dtree_model, feature_names=['objRed', 'objGreen', 'objBlue'], fontsize=10)  # Draw the tree with feature names
plt.savefig("dtree.jpg")  # Save the tree diagram as an image
```

> Example: Graphical representation of the Decision Tree trained on our dataset:

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docsdtree.jpg" />
</p>

#### Text-based rules

We can also export the tree as human-readable rules. 
-These rules reflect the actual thresholds learned by the model from dataset:

```Python 
rules = export_text(
    dtree_model,
    feature_names=['objRed', 'objGreen', 'objBlue']
)

print('\n' + rules + '\n')
```

> Example: Rules generated by the Decision Tree trained on our dataset:

```
|--- objBlue <= 8.50
|   |--- objGreen <= 1.50
|   |   |--- class: RED
|   |--- objGreen >  1.50
|   |   |--- objRed <= 7.50
|   |   |   |--- objBlue <= 1.50
|   |   |   |   |--- objRed <= 2.50
|   |   |   |   |   |--- class: GREEN
|   |   |   |   |--- objRed >  2.50
|   |   |   |   |   |--- class: YELLOW
|   |   |   |--- objBlue >  1.50
|   |   |   |   |--- objGreen <= 4.00
|   |   |   |   |   |--- class: PURPLE
|   |   |   |   |--- objGreen >  4.00
|   |   |   |   |   |--- class: GREEN
|   |   |--- objRed >  7.50
|   |   |   |--- objBlue <= 5.50
|   |   |   |   |--- objGreen <= 6.00
|   |   |   |   |   |--- class: RED
|   |   |   |   |--- objGreen >  6.00
|   |   |   |   |   |--- class: YELLOW
|   |   |   |--- objBlue >  5.50
|   |   |   |   |--- objGreen <= 43.00
|   |   |   |   |   |--- class: GREEN
|   |   |   |   |--- objGreen >  43.00
|   |   |   |   |   |--- class: YELLOW
|--- objBlue >  8.50
|   |--- objRed <= 2.50
|   |   |--- objGreen <= 5.00
|   |   |   |--- objBlue <= 9.50
|   |   |   |   |--- class: BLUE
|   |   |   |--- objBlue >  9.50
|   |   |   |   |--- class: PURPLE
|   |   |--- objGreen >  5.00
|   |   |   |--- class: BLUE
|   |--- objRed >  2.50
|   |   |--- objGreen <= 17.00
|   |   |   |--- objGreen <= 8.50
|   |   |   |   |--- class: PURPLE
|   |   |   |--- objGreen >  8.50
|   |   |   |   |--- objRed <= 4.50
|   |   |   |   |   |--- class: BLUE
|   |   |   |   |--- objRed >  4.50
|   |   |   |   |   |--- class: PURPLE
|   |   |--- objGreen >  17.00
|   |   |   |--- class: BLUE
```

## 3. Model Deployment

This chapter demonstrates how to deploy the trained Decision Tree model on the Leanbot. The workflow emphasizes using AI assistants (ChatGPT, Gemini, etc.) to convert the trained model into embedded C++ code, then testing it with real objects.

### 3.1. Convert Model into Leanbot Code with Generative AI

We use an AI assistant to turn the exported Decision Tree rules into a hand-written classification function for Arduino.

#### Recommended Workflow

1. **Train the Decision Tree in Python**
   - Use `export_text()` to get the rule-based tree (Introduced above)

2. **Copy the Tree Rules**
   - Paste the text output directly into ChatGPT / Gemini.

3. **Ask AI to Generate function**
   - Request a function like:
     - `classifyColor(int r, int g, int b)`
	 - return a string is the name of the color.
   - Specify:
     - Target language (C / Arduino)
     - Integer inputs only (no floats if desired)

#### Steps to Generate C-Style Classification Function Using AI

</ol>

<li value="1" style="list-style-type: none;"> 1. Open ChatGPT or Gemini.</li>
<li value="2" style="list-style-type: none;"> 2. Copy the text-based decision tree (from the trained model) and paste it into ChatGPT / Gemini.</li>

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/gpt.jpg" width="50%" />
</p>


<li value="3" style="list-style-type: none;"> 3. Ask AI to generate a function (floating-point thresholds) using this prompt:</li>

```Text
Convert the following decision tree into C code using nested if–else statements.
Output only a function with this exact signature and style: String classifyColor(float objRed, float objGreen, float objBlue)
```

=> The AI will output a C-style function like:

```C
String classifyColor(float objRed, float objGreen, float objBlue) {
    // --- objBlue <= 8.50
    if (objBlue <= 8.50) {
        // |--- objGreen <= 1.50
        if (objGreen <= 1.50) {
            // |   |--- class: RED
            return "RED";
        }
        // |--- objGreen > 1.50
        else {
            // |   |--- objRed <= 7.50
            if (objRed <= 7.50) {
                // |   |   |--- objBlue <= 1.50
                if (objBlue <= 1.50) {
                    // |   |   |   |--- objRed <= 2.50
                    if (objRed <= 2.50) {
                        // |   |   |   |   |--- class: GREEN
                        return "GREEN";
                    }
                    // |   |   |   |--- objRed > 2.50
                    else {
                        // |   |   |   |   |--- class: YELLOW
                        return "YELLOW";
                    }
                }
                // |   |   |--- objBlue > 1.50
                else {
                    // |   |   |   |--- objGreen <= 4.00
                    if (objGreen <= 4.00) {
                        // |   |   |   |   |--- class: PURPLE
                        return "PURPLE";
                    }
                    // |   |   |   |--- objGreen > 4.00
                    else {
                        // |   |   |   |   |--- class: GREEN
                        return "GREEN";
                    }
                }
            }
            // |   |--- objRed > 7.50
            else {
                // |   |   |--- objBlue <= 5.50
                if (objBlue <= 5.50) {
                    // |   |   |   |--- objGreen <= 6.00
                    if (objGreen <= 6.00) {
                        // |   |   |   |   |--- class: RED
                        return "RED";
                    }
                    // |   |   |   |--- objGreen > 6.00
                    else {
                        // |   |   |   |   |--- class: YELLOW
                        return "YELLOW";
                    }
                }
                // |   |   |--- objBlue > 5.50
                else {
                    // |   |   |   |--- objGreen <= 43.00
                    if (objGreen <= 43.00) {
                        // |   |   |   |   |--- class: GREEN
                        return "GREEN";
                    }
                    // |   |   |   |--- objGreen > 43.00
                    else {
                        // |   |   |   |   |--- class: YELLOW
                        return "YELLOW";
                    }
                }
            }
        }
    }
    // --- objBlue > 8.50
    else {
        // |--- objRed <= 2.50
        if (objRed <= 2.50) {
            // |   |--- objGreen <= 5.00
            if (objGreen <= 5.00) {
                // |   |   |--- objBlue <= 9.50
                if (objBlue <= 9.50) {
                    // |   |   |   |--- class: BLUE
                    return "BLUE";
                }
                // |   |   |--- objBlue > 9.50
                else {
                    // |   |   |   |--- class: PURPLE
                    return "PURPLE";
                }
            }
            // |   |--- objGreen > 5.00
            else {
                // |   |   |--- class: BLUE
                return "BLUE";
            }
        }
        // |--- objRed > 2.50
        else {
            // |   |--- objGreen <= 17.00
            if (objGreen <= 17.00) {
                // |   |   |--- objGreen <= 8.50
                if (objGreen <= 8.50) {
                    // |   |   |   |--- class: PURPLE
                    return "PURPLE";
                }
                // |   |   |--- objGreen > 8.50
                else {
                    // |   |   |   |--- objRed <= 4.50
                    if (objRed <= 4.50) {
                        // |   |   |   |   |--- class: BLUE
                        return "BLUE";
                    }
                    // |   |   |   |--- objRed > 4.50
                    else {
                        // |   |   |   |   |--- class: PURPLE
                        return "PURPLE";
                    }
                }
            }
            // |   |--- objGreen > 17.00
            else {
                // |   |   |--- class: BLUE
                return "BLUE";
            }
        }
    }
}
```

<li value="4" style="list-style-type: none;"> 4. Ask AI to generate a function (int thresholds) using this prompt:</li>

</ol>


```Text
Convert the decision tree directly into C code using integer comparisons only.
Assume objRed, objGreen, objBlue are integers.

Output only a function with this exact signature and style: String classifyColor(int objRed, int objGreen, int objBlue)

Rules:
DO NOT generate a float version first
DO NOT use floats, casts, rounding, ceil, or floor
Replace thresholds like <= 8.50 with the correct integer-safe condition
(e.g. <= 8.50 → <= 8, > 8.50 → >= 9)
Skip or collapse branches that become unreachable or redundant for integers
Preserve the exact logical intent of the original tree
Use clear nested if / else with comments showing the resolved integer condition
Return only: "RED", "GREEN", "BLUE", "YELLOW", "PURPLE"
No extra code
```

=> The AI will output a C-style function like:

```C 
String classifyColor(int objRed, int objGreen, int objBlue) {
    // Original: --- objBlue <= 8.50  | Integer: if (objBlue <= 8)
    if (objBlue <= 8) {
        // Original: |--- objGreen <= 1.50 | Integer: if (objGreen <= 1)
        if (objGreen <= 1) {
            // |   |--- class: RED
            return "RED";
        }
        // Original: |--- objGreen > 1.50 | Integer: else (objGreen >= 2)
        else {
            // Original: |   |--- objRed <= 7.50 | Integer: if (objRed <= 7)
            if (objRed <= 7) {
                // Original: |   |   |--- objBlue <= 1.50 | Integer: if (objBlue <= 1)
                if (objBlue <= 1) {
                    // Original: |   |   |   |--- objRed <= 2.50 | Integer: if (objRed <= 2)
                    if (objRed <= 2) {
                        // |   |   |   |   |--- class: GREEN
                        return "GREEN";
                    }
                    // Original: |   |   |   |--- objRed > 2.50 | Integer: else (objRed >= 3)
                    else {
                        // |   |   |   |   |--- class: YELLOW
                        return "YELLOW";
                    }
                }
                // Original: |   |   |--- objBlue > 1.50 | Integer: else (objBlue >= 2)
                else {
                    // Original: |   |   |   |--- objGreen <= 4.00 | Integer: if (objGreen <= 4)
                    if (objGreen <= 4) {
                        // |   |   |   |   |--- class: PURPLE
                        return "PURPLE";
                    }
                    // Original: |   |   |   |--- objGreen > 4.00 | Integer: else (objGreen >= 5)
                    else {
                        // |   |   |   |   |--- class: GREEN
                        return "GREEN";
                    }
                }
            }
            // Original: |   |--- objRed > 7.50 | Integer: else (objRed >= 8)
            else {
                // Original: |   |   |--- objBlue <= 5.50 | Integer: if (objBlue <= 5)
                if (objBlue <= 5) {
                    // Original: |   |   |   |--- objGreen <= 6.00 | Integer: if (objGreen <= 6)
                    if (objGreen <= 6) {
                        // |   |   |   |   |--- class: RED
                        return "RED";
                    }
                    // Original: |   |   |   |--- objGreen > 6.00 | Integer: else (objGreen >= 7)
                    else {
                        // |   |   |   |   |--- class: YELLOW
                        return "YELLOW";
                    }
                }
                // Original: |   |   |--- objBlue > 5.50 | Integer: else (objBlue >= 6)
                else {
                    // Original: |   |   |   |--- objGreen <= 43.00 | Integer: if (objGreen <= 43)
                    if (objGreen <= 43) {
                        // |   |   |   |   |--- class: GREEN
                        return "GREEN";
                    }
                    // Original: |   |   |   |--- objGreen > 43.00 | Integer: else (objGreen >= 44)
                    else {
                        // |   |   |   |   |--- class: YELLOW
                        return "YELLOW";
                    }
                }
            }
        }
    }
    // Original: --- objBlue > 8.50 | Integer: else (objBlue >= 9)
    else {
        // Original: |--- objRed <= 2.50 | Integer: if (objRed <= 2)
        if (objRed <= 2) {
            // Original: |   |--- objGreen <= 5.00 | Integer: if (objGreen <= 5)
            if (objGreen <= 5) {
                // Original: |   |   |--- objBlue <= 9.50 | Integer: if (objBlue <= 9)
                if (objBlue <= 9) {
                    // |   |   |   |--- class: BLUE
                    return "BLUE";
                }
                // Original: |   |   |--- objBlue > 9.50 | Integer: else (objBlue >= 10)
                else {
                    // |   |   |   |--- class: PURPLE
                    return "PURPLE";
                }
            }
            // Original: |   |--- objGreen > 5.00 | Integer: else (objGreen >= 6)
            else {
                // |   |   |--- class: BLUE
                return "BLUE";
            }
        }
        // Original: |--- objRed > 2.50 | Integer: else (objRed >= 3)
        else {
            // Original: |   |--- objGreen <= 17.00 | Integer: if (objGreen <= 17)
            if (objGreen <= 17) {
                // Original: |   |   |--- objGreen <= 8.50 | Integer: if (objGreen <= 8)
                if (objGreen <= 8) {
                    // |   |   |   |--- class: PURPLE
                    return "PURPLE";
                }
                // Original: |   |   |--- objGreen > 8.50 | Integer: else (objGreen >= 9)
                else {
                    // Original: |   |   |   |--- objRed <= 4.50 | Integer: if (objRed <= 4)
                    if (objRed <= 4) {
                        // |   |   |   |   |--- class: BLUE
                        return "BLUE";
                    }
                    // Original: |   |   |   |--- objRed > 4.50 | Integer: else (objRed >= 5)
                    else {
                        // |   |   |   |   |--- class: PURPLE
                        return "PURPLE";
                    }
                }
            }
            // Original: |   |--- objGreen > 17.00 | Integer: else (objGreen >= 18)
            else {
                // |   |   |--- class: BLUE
                return "BLUE";
            }
        }
    }
}
```

### 3.2. Integration into Leanbot program

This section shows how to integrate the AI-generated classifyColor() function into the Leanbot program and test it with real objects.

#### Introduce the Template Program

The Leanbot program comes with a template program that reads RGB values and classifies colors. Students will replace this placeholder with the AI-generated function
- The template program: [template.ino](https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/blob/master/Code/template.ino)

**How the Template Works**:
- The Leanbot reads object RGB values under red, green, blue, and ambient light.
- Values are stored in global variables: `objRed, objGreen, objBlue`.
- The template function is a placeholder returning a color string based on these values.

**Template Classification Function**:
- This is a placeholder function returning a color name string based on the RGB values.  
- Students should replace this template with the AI-generated function for detecting object colors.


```c
//==============================================================================
//  Colored-Object Detection (only Red, Green, Blue)
//==============================================================================

String classifyColor(float objRed, float objGreen, float objBlue) {
    return "RED"; // Example
}
```

#### Replacing the Template Function

- Replace the template function with the AI-generated function (using int thresholds).  
- Example code: [classifyColor.ino](https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/blob/master/Code/classifiyColor.ino)

```C 
void ObjectColorDemo() {
  Obj_readRGB();        // activate Led light and APDS color sensor to read object RGB values
 
  // Print out object RGB values for observation
  Serial.println();
  Serial.print(  "ObjRed   = " );
  Serial.println( ObjRed );
  Serial.print(  "ObjGreen = " );
  Serial.println( ObjGreen );
  Serial.print(  "ObjBlue  = " );
  Serial.println( ObjBlue );

  String color = classifyColor(ObjRed, ObjGreen, ObjBlue);
  Serial.print( "Detect Jenga Block: ");
  Serial.println(color);
}
```

### 3.3. Running on Leanbot

**Steps**:
1. Power on the Leanbot and connect via USB.
2. Open the Serial Monitor.
3. Place objects (Jenga blocks) between the grippers, within 5 cm.
4. Observe the printed RGB values and the detected color.
5. Observe the Leanbot RGB LED indicating the identified color.

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/d1.jpg" width="50%" />
</p>

**What Leanbot Does**:
- The Leanbot repeatedly flashes its RGB LED and analyzes the object’s color every 3 seconds.
- After a color is identified, the LED displays the detected color and remains on until the next measurement cycle.
- The detected color is:
  + Printed to the Serial Monitor (RGB values and color name)
  + Displayed on the Leanbot RGB LED

#### Example Outputs

The following tests demonstrate how the Leanbot detects object colors using the AI-generated `classifyColor()` function derived from the trained Decision Tree model.

##### Red Jenga block

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/dred.jpg" width="50%" />
</p>


- Serial Output:

```

ObjRed   = 19
ObjGreen = 1
ObjBlue  = 2
Detect Jenga Block: RED

ObjRed   = 19
ObjGreen = 2
ObjBlue  = 2
Detect Jenga Block: RED

ObjRed   = 19
ObjGreen = 2
ObjBlue  = 2
Detect Jenga Block: RED

```

##### Green Jenga block

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/dgreen.jpg" width="50%" />
</p>


- Serial Output:

```

ObjRed   = 6
ObjGreen = 23
ObjBlue  = 5
Detect Jenga Block: GREEN

ObjRed   = 6
ObjGreen = 23
ObjBlue  = 5
Detect Jenga Block: GREEN

ObjRed   = 6
ObjGreen = 23
ObjBlue  = 5
Detect Jenga Block: GREEN

```

##### Blue Jenga block

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/dblue.jpg" width="50%" />
</p>


- Serial Output:

```

ObjRed   = 2
ObjGreen = 14
ObjBlue  = 31
Detect Jenga Block: BLUE

ObjRed   = 2
ObjGreen = 14
ObjBlue  = 31
Detect Jenga Block: BLUE

ObjRed   = 2
ObjGreen = 15
ObjBlue  = 31
Detect Jenga Block: BLUE

```

##### Yellow Jenga block

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/dyellow.jpg" width="50%" />
</p>


- Serial Output:

```

ObjRed   = 24
ObjGreen = 27
ObjBlue  = 5
Detect Jenga Block: YELLOW

ObjRed   = 24
ObjGreen = 27
ObjBlue  = 5
Detect Jenga Block: YELLOW

ObjRed   = 24
ObjGreen = 27
ObjBlue  = 5
Detect Jenga Block: YELLOW

```

##### Purple Jenga block

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/dpurple.jpg" width="50%" />
</p>


- Serial Output:

```

ObjRed   = 4
ObjGreen = 5
ObjBlue  = 26
Detect Jenga Block: PURPLE

ObjRed   = 5
ObjGreen = 6
ObjBlue  = 26
Detect Jenga Block: PURPLE

ObjRed   = 4
ObjGreen = 6
ObjBlue  = 26
Detect Jenga Block: PURPLE

```

##### Blue Jenga block (misclassified as Purple)

<p style="text-align: center;">  
<img src="https://git.pythaverse.space/TrainingAIROC_2026/Advanced_Color_Sensor_with_AI/raw/master/Docs/dw.jpg" width="50%" />
</p>

Description:
- In this example, a Blue Jenga block is placed in front of the Leanbot, but the model incorrectly predicts the color as PURPLE.

This misclassification could be due to either:
- The Jenga block is positioned too far and off-center, leading to weak measurement signals.
- The classification model is not refined enough. Students are expected to explorer ways to improve their own model: 
  + More variety of training data, 
  + Tuning model training parameters, 
  + Trying other types of models beside Decision Tree, such as Random Forest, Multi Layer Perceptron, ect.

- Serial Output:

```

ObjRed   = 1
ObjGreen = 5
ObjBlue  = 12
Detect Jenga Block: PURPLE

ObjRed   = 1
ObjGreen = 4
ObjBlue  = 12
Detect Jenga Block: PURPLE

ObjRed   = 1
ObjGreen = 5
ObjBlue  = 12
Detect Jenga Block: PURPLE

```
