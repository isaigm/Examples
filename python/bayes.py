import pandas as pd

data = pd.read_csv("iris.csv")
train_set = data.sample(frac=0.7, random_state=125) 
test_set = data.drop(train_set.index) 

class bayes_classifier:

    def fit(self, train: pd.DataFrame):
        self.number_of_rows = train.shape[0]
        self.target_column = train[train.columns[-1]].value_counts().to_dict()
        self.columns_except_last = train.columns[:-1]
        last_column = train.columns[-1]
        self.results = {}
        for col in self.columns_except_last:
            result = train.groupby([col, last_column]).size().unstack(fill_value=0)
            self.results[col] = result.to_dict()
    
    def predict(self, input_test):
        max_prob = float('-inf')
        predicted_target = ''
        for (target, freq) in self.target_column.items():
            py = freq / self.number_of_rows
            probability = py
            for (value, col) in zip(input_test[:-1], self.columns_except_last):
                freq_value_given_class = self.results[col][target].get(value, 0) + 1
                prob = freq_value_given_class / freq
                probability *= prob
            if max_prob < probability:
                predicted_target = target
                max_prob = probability
        
        return predicted_target

    def accuracy(self, dataset):
        success = 0
        for i in range(dataset.shape[0]):
            predicted = self.predict(dataset.iloc[i].to_numpy())
            if dataset.iloc[i, -1] == predicted:
                success += 1
        return success / dataset.shape[0]

bayes = bayes_classifier()
bayes.fit(train_set)
acc = bayes.accuracy(test_set)
print(acc)
