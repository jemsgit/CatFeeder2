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
                    },
                    {
                        loader: 'postcss-loader',
                        options: {
                            ident: 'postcss',
                            plugins: [
                                require('postcss-omit-import-tilde')(),
                                require('postcss-import')(),
                                require('postcss-url')({
                                    url: 'rebase'
                                }),
                                require('postcss-mixins')(),
                                require('postcss-for')(),
                                require('postcss-each')(),
                                require('postcss-custom-properties')({
                                    preserve: false
                                }),
                                require('postcss-strip-units')(),
                                require('postcss-calc')(),
                                require('postcss-color-function')(),
                                require('postcss-nested')(),
                                require('autoprefixer')(),
                                require('postcss-inherit')
                            ]
                        }
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