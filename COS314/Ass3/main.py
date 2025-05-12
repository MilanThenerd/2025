import pandas as pd
from sklearn.neural_network import MLPClassifier
from sklearn.metrics import accuracy_score

train_df = pd.read_csv('./data/BTC_train.csv')

X_train = train_df[['Open', 'High', 'Low', 'Close', 'Adj Close']]
y_train = train_df['Output']

test_df = pd.read_csv('./data/BTC_test.csv')
X_test = test_df[['Open', 'High', 'Low', 'Close', 'Adj Close']]
y_test = test_df['Output']

random_states = range(10, 110, 10)
results = []

for state in random_states:
    mlp = MLPClassifier(
        hidden_layer_sizes=(10, 10),
        activation='relu',
        solver='adam',
        max_iter=10000,
        random_state=state
    )
    mlp.fit(X_train, y_train)

    y_pred = mlp.predict(X_test)

    accuracy = accuracy_score(y_test, y_pred)
    results.append((state, accuracy))

results.sort(key=lambda x: x[1], reverse=True)

best_seed = results[0]
median_seed = results[len(results) // 2]
worst_seed = results[-1]

print(f"Best seed: {best_seed[0]} with Accuracy: {best_seed[1] * 100:.2f}%")
print(f"Median seed: {median_seed[0]} with Accuracy: {median_seed[1] * 100:.2f}%")
print(f"Worst seed: {worst_seed[0]} with Accuracy: {worst_seed[1] * 100:.2f}%")
