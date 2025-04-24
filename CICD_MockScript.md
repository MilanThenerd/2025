Skeleton/Un finished workflow code
```name: MPDB NoSQL CI/CD

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  test-and-build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    
    - name: Set up Node.js
      uses: actions/setup-node@v3
      with:
        node-version: '18'
        cache: 'npm'
        
    - name: Install dependencies
      run: npm ci
      
    - name: Run linting
      run: npm run lint || true
      
    - name: Run unit tests
      run: npm test
      
    - name: Build project
      run: npm run build

  deploy-documentation:
    needs: test-and-build
    if: github.ref == 'refs/heads/main' && github.event_name == 'push'
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    
    - name: Set up Node.js
      uses: actions/setup-node@v3
      with:
        node-version: '18'
        
    - name: Install dependencies
      run: npm ci
      
    - name: Generate documentation
      run: npm run docs
      
    - name: Deploy to GitHub Pages
      uses: JamesIves/github-pages-deploy-action@v4
      with:
        folder: docs
        branch: gh-pages

  build-image:
    needs: test-and-build
    if: github.ref == 'refs/heads/main' && github.event_name == 'push'
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3

    - name: Set up Docker Buildx
      uses: docker/setup-buildx-action@v2

    - name: Build Docker image
      uses: docker/build-push-action@v3
      with:
        context: .
        push: false
        tags: mpdb/nosql:latest
        outputs: type=docker,dest=/tmp/mpdb-image.tar

    - name: Upload image as artifact
      uses: actions/upload-artifact@v3
      with:
        name: mpdb-docker-image
        path: /tmp/mpdb-image.tar```
```{
  "name": "mp7",
  "version": "1.0.0",
  "type": "module",
  "description": "",
  "main": "index.js",
  "scripts": {
    "daemon": "node ./daemon/daemon.js",
    "cli": "node ./daemon/tempCLI.js",
    "testCommand": "node ./daemon/commandTest.js",
    "testStorageEngine": "node ./daemon/storageEngineTest.js",
    "testConcurrency": "node ./daemon/testConcurrency.js",
    "testCommandWithDaemon": "node ./daemon/commandIntergrationTest.js"
  },
  "repository": {
    "type": "git",
    "url": "git+https://github.com/COS301-SE-2025/MP7.git"
  },
  "keywords": [],
  "author": "",
  "license": "ISC",
  "bugs": {
    "url": "https://github.com/COS301-SE-2025/MP7/issues"
  },
  "homepage": "https://github.com/COS301-SE-2025/MP7#readme",
  "dependencies": {
    "async-mutex": "^0.5.0",
    "socket.io": "^4.8.1",
    "socket.io-client": "^4.8.1"
  },
  "devDependencies": {
    "jsdoc": "^4.0.4"
  }
}
```
