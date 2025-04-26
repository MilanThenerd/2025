# MP7
Install dependencies: npm install

Run all tests: npm test

Run a specific test: npx jest __tests__/apiLibrary.test.js

Generate a test coverage report: npx jest --coverage

More info:

Testing Strategy
The tests use Jest's mocking capabilities to mock the fetch API, preventing any actual network requests. This allows us to:

-Test API calls without needing a real server
-Verify correct URL and payload construction
-Test error handling
-Test complete workflows

Each test follows this pattern:

-Setup (create API wrapper, mock responses)
-Execute method under test
-Verify correct behavior (API calls, error handling)