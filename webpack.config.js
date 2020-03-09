const path = require('path');
const VueLoaderPlugin = require('vue-loader/lib/plugin')

module.exports = {
    mode: 'development',
    entry: './src/index.js',
    output: {
        path: path.resolve(__dirname, 'www'),
        filename: 'index.bundle.js'
    },
    devtool: 'inline-source-map',
    resolve: {
        alias: {
            'vue$': 'vue/dist/vue.esm.js'
        },
        extensions: ['*', '.js', '.vue', '.json']
    },
    plugins: [new VueLoaderPlugin()],
    module: {
        rules: [{
                test: /\.(js)$/,
                exclude: /node_modules/,
                use: {
                    loader: 'babel-loader'
                }
            },
            {
                test: /\.css$/,
                use: [
                    'vue-style-loader',
                    {
                        loader: 'css-loader'
                    }
                ]
            },
            {
                test: /\.vue$/,
                loader: 'vue-loader'
            }
        ]
    }
}