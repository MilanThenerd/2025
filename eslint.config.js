import js from '@eslint/js';
import globals from 'globals';
import react from 'eslint-plugin-react';
import reactHooks from 'eslint-plugin-react-hooks';
import reactRefresh from 'eslint-plugin-react-refresh';

export default [
    { ignores: ['dist', 'node_modules', '*.min.js', 'out/**', '**/*.html', 'mp7/**'] },

    // Base config
    {
        files: ['**/*.js'],
        languageOptions: {
            ecmaVersion: 2022,
            sourceType: 'module',
            globals: {
                ...globals.node,
            }
        },
        rules: {
            ...js.configs.recommended.rules
        }
    },

    {
        files: ['**/*.test.js', '**/*.tests.js', '**/tests/**/*.js', '**/__tests__/**/*.js'],
        languageOptions: {
            globals: {
                ...globals.node,
                ...globals.jest, 
            }
        }
    },

    // daemon, api folders
    {
        files: ['daemon/**/*.js', 'api/**/*.js'],
        languageOptions: {
            globals: {
                ...globals.node,
            }
        },
        rules: {
            
        }
    },

    // UI/React rules
    {
        files: ['src/**/*.{js,jsx}', 'public/**/*.{js,jsx}'],
        languageOptions: {
            ecmaVersion: 2020,
            globals: globals.browser,
            parserOptions: {
                ecmaFeatures: { jsx: true },
            },
        },
        settings: { react: { version: '18.3' } },
        plugins: {
            react,
            'react-hooks': reactHooks,
            'react-refresh': reactRefresh,
        },
        rules: {
            ...react.configs.recommended.rules,
            ...react.configs['jsx-runtime'].rules,
            ...reactHooks.configs.recommended.rules,
            'react/jsx-no-target-blank': 'off',
            'react-refresh/only-export-components': [
                'warn',
                { allowConstantExport: true },
            ],
        },
    },
];