require 'securerandom'

class PicturesController < ApplicationController
  def index
  end

  def create
    params = picture_params
    file = create_tempfile(params[:file])

    picture = Picture.new(upload: file)
    if picture.save
      render json: {}, status: 200
    else
      render json: { error: "something went wrong" }, status: 400
    end

    ensure
    delete_tempfile
  end

  private

  def picture_params
    params.require(:picture).permit(:file)
  end

  def create_tempfile(base64_image)
    content_type, data = base64_image.split(/:/)[0..1]
    file_extension = Rack::Mime::MIME_TYPES.invert[content_type]
    filename = "#{SecureRandom.uuid.gsub(/\-/, '')}#{file_extension}"
    
    @tempfile = Tempfile.new(filename)
    @tempfile.binmode
    @tempfile.write Base64.decode64(data)
    @tempfile.rewind

    ActionDispatch::Http::UploadedFile.new({
      tempfile: @tempfile,
      content_type: content_type,
      filename: filename
    })
  end

  def delete_tempfile
    @tempfile.close
    @tempfile.unlink
  end
end
