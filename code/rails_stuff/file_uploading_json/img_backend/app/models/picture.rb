class Picture < ActiveRecord::Base
  mount_uploader :upload, UploadUploader
end
