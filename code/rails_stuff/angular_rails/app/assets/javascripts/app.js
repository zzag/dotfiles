var app = angular.module("ang-rails", ["ui.router"]).
	config(['',function() {
		
	}]).
	factory('posts', [function(){
		var o = {
			posts: [
				{
					"title": "Hello!"
				},
				{
					"title": "Second post!"
				}
			]
		};

		return o;
	}]).
	controller('MainController', ['$scope', 'posts', function($scope, posts){
		$scope.posts = posts.posts;
	}]);