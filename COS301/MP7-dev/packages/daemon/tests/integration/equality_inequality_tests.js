// daemon/Tests/equality_inequality_tests.js
import assert from 'assert';
import StorageEngine from '../StorageEngine.js';

// Debug: list all static properties (methods and data) on StorageEngine
console.log("StorageEngine own keys:", Reflect.ownKeys(StorageEngine));
console.log("matchDocument exists?", typeof StorageEngine.matchDocument);

// Run tests for equality and inequality query operations
async function runTests() {
  console.log("Running Equality and Inequality Tests...");

  // Sample documents for testing
  const people = [
    { name: "John", age: 25, city: "New York" },
    { name: "Alice", age: 30, city: "Los Angeles" },
    { name: "Bob", age: 25, city: "New York" },
    { name: "John", age: 35, city: "Chicago" },
    { profile: { name: "John", age: 25 }, status: "active" } // no top-level "city"
  ];

  let query, results;

  // Test 1: Equality on name and age: Only one document should match.
  query = { name: { "$eq": "John" }, age: { "$eq": 25 } };
  results = people.filter(person => StorageEngine.matchDocument(person, query));
  assert.deepStrictEqual(
    results,
    [{ name: "John", age: 25, city: "New York" }],
    "Test 1 failed: Expected one matching document where name is 'John' and age equals 25"
  );

  // Test 2: Equality on age only: Two documents should match.
  query = { age: { "$eq": 25 } };
  results = people.filter(person => StorageEngine.matchDocument(person, query));
  assert.deepStrictEqual(
    results,
    [
      { name: "John", age: 25, city: "New York" },
      { name: "Bob", age: 25, city: "New York" }
    ],
    "Test 2 failed: Expected two matching documents where age equals 25"
  );

  // Test 3: Equality on name and inequality on age: Expect document with age not equal to 25.
  query = { name: { "$eq": "John" }, age: { "$ne": 25 } };
  results = people.filter(person => StorageEngine.matchDocument(person, query));
  assert.deepStrictEqual(
    results,
    [{ name: "John", age: 35, city: "Chicago" }],
    "Test 3 failed: Expected one matching document where name is 'John' and age is not 25"
  );

  // Test 4: Inequality on city: Documents with a city field not equal to "Chicago" should match.
  // Note: The document without a top-level "city" will have undefined and will be included since undefined !== "Chicago".
  query = { city: { "$ne": "Chicago" } };
  results = people.filter(person => StorageEngine.matchDocument(person, query));
  assert.deepStrictEqual(
    results,
    [
      { name: "John", age: 25, city: "New York" },
      { name: "Alice", age: 30, city: "Los Angeles" },
      { name: "Bob", age: 25, city: "New York" },
      { profile: { name: "John", age: 25 }, status: "active" }
    ],
    "Test 4 failed: Expected documents where 'city' is not 'Chicago'"
  );

  // Test 5: Simple equality check without using an operator.
  query = { name: "Alice" };
  results = people.filter(person => StorageEngine.matchDocument(person, query));
  assert.deepStrictEqual(
    results,
    [{ name: "Alice", age: 30, city: "Los Angeles" }],
    "Test 5 failed: Expected one matching document where name is 'Alice'"
  );

  // Test 6: Nested field query using dot-notation.
  query = { "profile.name": { "$eq": "John" }, "profile.age": { "$eq": 25 } };
  results = people.filter(person => StorageEngine.matchDocument(person, query));
  assert.deepStrictEqual(
    results,
    [{ profile: { name: "John", age: 25 }, status: "active" }],
    "Test 6 failed: Expected one matching document for nested field query"
  );

  console.log("All equality and inequality tests passed successfully.");
}

runTests().catch(error => {
  console.error("Tests failed:", error);
});
