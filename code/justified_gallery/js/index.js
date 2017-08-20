function justifyRow(images, sizes, startIndex, options) {
	var ratio = options["row"] / options["total"];
	$.each(images, function(index, image) {
		var imageWidth = sizes[index + startIndex];
		var newWidth = Math.floor(ratio * imageWidth);
		$(image).outerWidth(newWidth);
	});
}


function makeAllImagesTheSameHeight(images, options) {
	var allImageWidth = [];

	images.each(function() {
		var imageHeight = $(this).outerHeight();
		var imageWidth = $(this).outerWidth();
		var newWidth = Math.floor(options["max_height"] / imageHeight * imageWidth);
		allImageWidth.push(newWidth);
	});

	return allImageWidth;
}


function adjustHeightToFillRow(images, width_info, options) {
	var rowImages = [];
	var sizes = {
		total: 0,
		row: options["row_width"]
	};
	var perRowCurrent = 0;
	var startIndex = 0;

	$.each(images, function(index, image) {
		rowImages.push(image);
		sizes["total"] += width_info[index];
		perRowCurrent += 1;

		if (sizes["total"] >= sizes["row"] || options["per_row"] <= perRowCurrent) {
			startIndex = index - rowImages.length + 1;
			justifyRow(rowImages, width_info, startIndex, sizes);

			rowImages = [];
			sizes["total"] = 0;
			perRowCurrent = 0;
		}
	});

	if (rowImages.length != 0) {
		startIndex = images.length - rowImages.length;
		justifyRow(rowImages, width_info, startIndex, sizes);
	}
}


function justifyGallery(gallery) {
	var options = {
		max_height: 128,
		per_row: 5,
		row_width: $(gallery).width()
	};
	var allImageWidth = [];

	images = $(gallery).children();

	allImageWidth = makeAllImagesTheSameHeight(images, options);
	adjustHeightToFillRow(images, allImageWidth, options);
}