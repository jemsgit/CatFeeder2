module.exports = {
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